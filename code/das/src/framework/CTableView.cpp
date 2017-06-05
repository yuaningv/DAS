#include "CTableView.h"
#include "CFrozenTableView.h"
#include "CFileOperationManager.h"
#include "constant.h"

#include "QtWidgets/QVBoxLayout"
#include "QtGui/QStandardItemModel"
#include "QtWidgets/QHeaderView"
#include "QtCore/QEvent"
#include "QtGui/QMouseEvent"
#include "QtWidgets/QPushButton"
#include "QtWidgets/QFileDialog"
#include "QtWidgets/QMessageBox"
#include "QtCore/QThread"
#include "QtWidgets/QScrollBar"

#include "CLogManager.h"


const char* cstExportSuccess = "Export Success!";
const char* cstExportFailed = "Export Failed!";
const char* cstTips = "Tips";
const char* cstExport = "Export excel";

CTableView::CTableView(QWidget* parent /*= 0*/)
    : CCustomWidgetBase(parent)
    , m_pTableView(nullptr)
    , m_pModel(nullptr)
    , m_pExcelBtn(nullptr)
    , m_pLbTitle(nullptr)
    , m_strStartTime("")
    , m_strEndTime("")
    , m_pThread(nullptr),
    m_iChannel(1)
{
    m_lstHorizontalHeader.clear();

    m_pThread = new QThread(this);

    initLayout();
}

CTableView::~CTableView()
{

}


// 初始化布局
void CTableView::initLayout()
{
    static int ide = 1;
    m_pModel = new CTableModel(this);
    createHorizontalHeaders();

    m_pTableView = new CFrozenTableView(m_pModel, 1);  // 首列冻结
    m_pTableView->resizeColumnsToContents();
    m_pTableView->setColumnWidth(0, 160);
    //m_pTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    //m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_pTableView->setAlternatingRowColors(true);

    m_pLbTitle = new QLabel(QString("Table %1").arg(ide++), this);

    QVBoxLayout* pTmpLayout = new QVBoxLayout(this);
    pTmpLayout->addWidget(m_pLbTitle);
    pTmpLayout->addWidget(m_pTableView);

    m_pExcelBtn = new QPushButton(trFormString(cstExport), this);
    connect(m_pExcelBtn, &QPushButton::clicked, this, &CTableView::OnExport);

    pTmpLayout->addWidget(m_pExcelBtn, 0, Qt::AlignRight);

    this->setLayout(pTmpLayout);
}


// 填充表格数据
void CTableView::setTableViewData()
{

}

void CTableView::createHorizontalHeaders()
{
    // 读取xml
    CFileOperationManager cfm("candatadefine.xml");
    if (!cfm.ReadXmlFile(m_lstHorizontalHeader))
    {
        return;
    }

    // 添加时间头
    CurveLine_t TimeHeader;
    TimeHeader.m_strName = "time";
    TimeHeader.m_strDisplayName = QStringLiteral("时间");
    m_lstHorizontalHeader.push_front(TimeHeader);

    m_pModel->setColumnCount(m_lstHorizontalHeader.size());
    for (int i = 0; i < m_lstHorizontalHeader.count(); ++i)
    {
        QStandardItem* pItem = new QStandardItem(m_lstHorizontalHeader.at(i).m_strDisplayName);
        pItem->setData(m_lstHorizontalHeader.at(i).m_strName, CurveRole);
        m_pModel->setHorizontalHeaderItem(i, pItem);
    }
}


ITEMTYPE CTableView::type()
{
    return Item_Table;
}


HWND CTableView::GetWndHandle()
{
    CLogManager::getInstance()->log(eLogDebug, "CTableView::GetWndHandle", "update frame, hwnd:%d", winId());
    return (HWND)this->winId();
}


void CTableView::OnMedia(unsigned char* buffer, unsigned long length, unsigned long payload,
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

    insertRowData(lstCanData);
    m_mutex.unlock();
}


void CTableView::setHorizontalHeader(QList<CurveLine_t>& lstHeader)
{
    m_lstHorizontalHeader = lstHeader;

    createHorizontalHeaders();
}


void CTableView::insertRowData(QList<CurveLine_t>& lstRowData)
{
    int iRow = m_pModel->rowCount();
    m_pModel->insertRow(iRow);
    m_pTableView->verticalScrollBar()->setValue(m_pTableView->verticalScrollBar()->maximum());

    // 复制上一次的数据
    if (iRow > 0)
    {
        for (int iCol = 0; iCol < m_pModel->columnCount(); ++iCol)
        {
            m_pModel->setData(m_pModel->index(iRow, iCol), m_pModel->data(m_pModel->index(iRow - 1, iCol)));
        }
    }

    // 设置当前数据
    for (int iCol = 0; iCol < m_pModel->columnCount(); ++iCol)
    {
        for (auto& TmpData : lstRowData)
        {
            if (TmpData.m_strDisplayName == m_pModel->horizontalHeaderItem(iCol)->text().trimmed())
            {
                m_pModel->setData(m_pModel->index(iRow, iCol), TmpData.m_strValue);
                break;
            }
        }
    }
}


/*
* 设置开始结束时间
* strStart 开始时间 strEnd 结束时间  时间格式 yyyy/MM/dd hh:mm:ss
*/
void CTableView::setTimeRange(const QString& strStart, const QString& strEnd)
{
    m_strStartTime = strStart;
    m_strEndTime = strEnd;
}


// 导出excel
void CTableView::OnExport()
{
    QString strFileName = QFileDialog::getSaveFileName(nullptr, trFormString(cstExport), "./", "*.xlsx");
    if (strFileName.isEmpty())
    {
        return;
    }

    if (QFileInfo(strFileName).suffix().isEmpty())
    {
        strFileName += ".xlsx";
    }

    // 表头
    QStringList slstHeader;
    for (int i = 0; i < m_lstHorizontalHeader.count(); ++i)
    {
        slstHeader << m_lstHorizontalHeader.at(i).m_strDisplayName;
    }

    // 数据源
    QList<QList<QVariant> > lstData;
    for (int iRow = 0; iRow < m_pModel->rowCount(); ++iRow)
    {
        QList<QVariant> lstTmpCol;
        for (int iCol = 0; iCol < m_pModel->columnCount(); ++iCol)
        {
            lstTmpCol << m_pModel->data(m_pModel->index(iRow, iCol));
        }
        lstData << lstTmpCol;
    }

    CFileOperationManager oTmpFileMananger;
    oTmpFileMananger.moveToThread(m_pThread);
    m_pThread->start();
    if (!oTmpFileMananger.writeExcelFile(strFileName, slstHeader, lstData))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(trFormString(cstTips));
        msgBox.setText(trFormString(cstExportFailed));
        msgBox.exec();
        return;
    }

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(trFormString(cstTips));
    msgBox.setText(trFormString(cstExportSuccess));
    msgBox.exec();
}
