#include "CExportCanData.h"
#include "CFileOperationManager.h"

#include "QtWidgets/QMessageBox"

#include "CLogManager.h"


extern const char* cstDictDas;
extern const char* cstExportSuccess;
extern const char* cstExportFailed;
extern const char* cstTips;


CExportCanData::CExportCanData()
{
    m_iChannel = 1;

    m_profile.Init("./candatadefine.xml");
    m_pCanDataExportSession = new CCanFileExportSession;
    connect(this, SIGNAL(sigCanDataUpdate(const QList<CurveLine_t>&)), this, SLOT(OnExport(const QList<CurveLine_t>&)));

    m_progressDialog.setWindowTitle(trFormString(cstDictDas));
    m_progressDialog.setLabelText("Export Can Data...");
    m_progressDialog.setCancelButtonText("Cancel");
    m_progressDialog.setRange(0, 100000);
    m_progressDialog.setWindowModality(Qt::ApplicationModal);
    m_progressDialog.resize(350, 125);
    connect(&m_progressDialog, SIGNAL(canceled()), this, SLOT(OnCancel()));

    m_xlsx = NULL;
    m_xlsxFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
    m_xlsxFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    m_xlsxFormat.setBorderStyle(QXlsx::Format::BorderThin);

    // test
#ifdef EXPORT_TEST
    m_iProgressTest = 0;
    m_pTimer = new QTimer;
    m_pTimer->setInterval(1000);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(OnTimer()));
    m_pTimer->start();
#endif


#ifndef EXPORT_TEST
    // 读取xml
    CFileOperationManager cfm("./candatadefine.xml");
    if (!cfm.ReadXmlFile(m_lstHeaderData))
    {
        return;
    }
#endif

    // 添加时间头
    CurveLine_t timeHeader;
    timeHeader.m_strName = "time";
    timeHeader.m_strDisplayName = QStringLiteral("时间");
    m_lstHeaderData.push_front(timeHeader);
}


CExportCanData::~CExportCanData()
{
    if (m_pCanDataExportSession != NULL)
    {
        m_pCanDataExportSession->Deinit();

        delete m_pCanDataExportSession;
        m_pCanDataExportSession = NULL;
    }
}


void CExportCanData::setChannel(int iChannel)
{
    m_iChannel = iChannel;
}


void CExportCanData::setInitData(int iChannel, const QString& strCanStoragePath)
{
    m_iChannel = iChannel;
    m_strCanStoragePath = strCanStoragePath;

    m_pCanDataExportSession->Init(m_strCanStoragePath.toStdString().c_str(), &m_profile, iChannel);
    m_pCanDataExportSession->GetStreamMgr()->AddStream(this);
}


/*
* 设置开始结束时间
* strStart 开始时间 strEnd 结束时间  时间格式 yyyyMMddhhmmss
*/
void CExportCanData::setTimeRange(const QDateTime& dtStart, const QDateTime& dtEnd)
{
    m_dtStartTime = dtStart;
    m_dtEndTime = dtEnd;

    QString strTime = dtStart.toString("yyyyMMddhhmmss") + "-" + dtEnd.toString("yyyyMMddhhmmss");
    m_pCanDataExportSession->SetScape(strTime.toStdString().c_str());
}


void CExportCanData::play()
{
    m_pCanDataExportSession->Play();
}


void CExportCanData::setExcelFileName(const QString& strExcelFileName)
{
    m_strExcelFileName = strExcelFileName;

    m_xlsx = new QXlsx::Document(m_strExcelFileName);
    for (int i = 0; i < m_lstHeaderData.size(); ++i)
    {
        if (!m_xlsx->write(1, i + 1, m_lstHeaderData.at(i).m_strDisplayName, m_xlsxFormat))   // 第一行表头
        {
            return;
        }
    }
    m_xlsx->save();
}


void CExportCanData::OnMedia(unsigned char* buffer, unsigned long length, unsigned long payload,
	CCustomDateTime* pTime, void* pCustomData)
{
    // can data 
    QList<CurveLine_t> lstCanData;
    CurveLine_t canData;

    m_mutex.lock();
    // time
    canData.m_strName = "time";
    canData.m_strDisplayName = QStringLiteral("时间");
	canData.m_strValue.sprintf("%04d/%02d/%02d %02d:%02d:%02d.%03d", pTime->year, pTime->month, pTime->mday, pTime->hour, pTime->min, pTime->sec, pTime->msec);
    lstCanData.push_back(canData);

    //char buf[256];
    CCanData* pData = (CCanData*)buffer;
    for (int i = 0; i < length; i++)
    {
        canData.m_strDisplayName = QString::fromLocal8Bit(pData[i].m_pCanItem->m_DispName.c_str());
        canData.m_strName = QString::fromStdString(pData[i].m_pCanItem->m_Name);
        canData.m_strValue = QString::number(pData[i].m_Value, 'f', 2);
        lstCanData.append(canData);
    }

    m_dtCurrentTime = QDateTime::fromString("yyyy/MM/dd hh:mm:ss");
    emit sigCanDataUpdate(lstCanData);
    m_mutex.unlock();
}


// 导出excel
void CExportCanData::OnExport(const QList<CurveLine_t>& lstCanData)
{
    // 设置进度条进度 

#ifndef EXPORT_TEST
    qint64 iCurrentTime = m_dtCurrentTime.toMSecsSinceEpoch();
    qint64 iMaxTime = m_dtStartTime.toMSecsSinceEpoch();
    qint64 iMinTime = m_dtEndTime.toMSecsSinceEpoch();
    qint64 iTimeOffset = iMaxTime - iMinTime;
    uint iOffset = m_progressDialog.maximum() - m_progressDialog.minimum();
    if (iTimeOffset <= 0)
    {
        return;
    }
    int iProgressValue = m_progressDialog.minimum() + (iCurrentTime - iMinTime) * (1.0f * iOffset / iTimeOffset);
#else
    int iProgressValue = m_iProgressTest += 10000;
#endif

    if (iProgressValue > m_progressDialog.maximum())
    {
#ifdef EXPORT_TEST
        m_pTimer->stop();
#endif
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(trFormString(cstTips));
        msgBox.setText(trFormString(cstExportSuccess));
        msgBox.exec();

        return;
    }
    m_progressDialog.setValue(iProgressValue);

    // excel data 
    QList<QList<QVariant> > lstExcelData;
    QList<QVariant> lstColData;
    int iSize = m_lstHeaderData.size();
    for (int iCol = 0; iCol < iSize; ++iCol)
    {
        for (auto& TmpData : lstCanData)
        {
            if (TmpData.m_strDisplayName == m_lstHeaderData.at(iCol).m_strDisplayName)
            {
                lstColData.append(TmpData.m_strValue);
                break;
            }
        }
    }
    lstExcelData.append(lstColData);

    // 写入本地EXCEL 
    for (int iRow = 0; iRow < lstExcelData.size(); ++iRow)
    {
        for (int iCol = 0; iCol < lstColData.size(); ++iCol)
        {
            if (!m_xlsx->write(m_xlsx->dimension().lastRow() + iRow + 1, iCol + 1, lstExcelData.at(iRow).at(iCol).toString().replace("T", " "), m_xlsxFormat))
            {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setWindowTitle(trFormString(cstTips));
                msgBox.setText(trFormString(cstExportFailed));
                msgBox.exec();
                return;
            }
        }
    }
    m_xlsx->save();
}


void CExportCanData::OnCancel()
{
    m_pCanDataExportSession->Pause();

#ifdef EXPORT_TEST
    m_pTimer->stop();
#endif
}


// test 
#ifdef EXPORT_TEST
void CExportCanData::OnTimer()
{
    QList<CurveLine_t> lstCanData;
    CurveLine_t canData;
    canData.m_strName = "time";
    canData.m_strDisplayName = QStringLiteral("时间");
    canData.m_strValue = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    lstCanData.push_back(canData);

    OnExport(lstCanData);
}
#endif // EXPORT_TEST


