#include "CCanDataListener.h"

#include "QtCore/QDateTime"


CCanDataListener::CCanDataListener(QObject *parent)
    : QObject(parent)
{
}


CCanDataListener::~CCanDataListener()
{
    m_profile.DeInit();
    m_canSession.Deinit();
}


int CCanDataListener::init(const QString& strStorage, const QString& strProfile, int iChannel)
{
    m_profile.Init(strProfile.toStdString().c_str());
    m_canSession.Init(strStorage.toStdString().c_str(), &m_profile, iChannel);
    m_canSession.GetStreamMgr()->AddStream(this);

    return 1;
}


int CCanDataListener::setScape(const QDateTime& dtBegin, const QDateTime& dtEnd)
{
    // time format eg:20170527143910-20170528143910
    QString strTime = dtBegin.toString("yyyyMMddhhmmss") + "-" + dtEnd.toString("yyyyMMddhhmmss");
    m_canSession.SetScape(strTime.toStdString().c_str());

    return 1;
}


int CCanDataListener::skipTo(const QDateTime& dtPos)
{
    QString strCurrentTime = dtPos.toString("yyyyMMddhhmmss");
    m_canSession.SkipTo(strCurrentTime.toStdString().c_str());

    return 1;
}


int CCanDataListener::play()
{
    m_canSession.Play();

    return 1;
}


int CCanDataListener::pause()
{
    m_canSession.Pause();

    return 1;
}


// 对于视频的接接收者，还需要关心这个图像数据的高宽，pCustomData 
// 指向了一个struct{ unsigned int width; unsigned int height; } 类型的数据 
// payload 区别can和video 
// buffer：对于视频接收者，为解码后的yuv420p的数据缓冲区；对于Can数据接收者，为CCanData类型的缓冲区 
// length： 对于视频，为解码后的长度，对于Can数据接收为CCanData的数据数量 
// secs：为距离1970以来的秒数【一个很大的数】，usecs为毫秒数【范围0 - 1000】 
void CCanDataListener::OnMedia(unsigned char* buffer, unsigned long length, unsigned long payload,
    unsigned long secs, unsigned long msecs, void* pCustomData)
{
    // 1 - video data, 3 - can data 
    if (payload == 1)   // video data 
    {
        return;
    }
    else if (payload == 3)  // can data 
    {
        // can data 
        QList<CurveLine_t> lstCanData;
        CurveLine_t canData;

        char buf[256];
        CCanData* pData = (CCanData*)buffer;
        for (int i = 0; i < length; i++)
        {
            canData.m_strDisplayName = QString::fromStdString(pData[i].m_pCanItem->m_DispName);
            canData.m_strName = QString::fromStdString(pData[i].m_pCanItem->m_Name);
            canData.m_strValue = QString::number(pData[i].m_Value, 'f', 2);
            lstCanData.append(canData);
        }

        // current time 
        QDateTime currentDateTime = QDateTime::fromMSecsSinceEpoch(secs * 1000 + msecs);

        emit sigCanUpdate(lstCanData, currentDateTime);
    }
}

