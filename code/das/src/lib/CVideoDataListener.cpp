#include "CVideoDataListener.h"

#include "QtCore/QDateTime"
#include "QtGui/QImage"

#include "type.h"


CVideoDataListener::CVideoDataListener(QObject *parent)
    : QObject(parent)
{
}


CVideoDataListener::~CVideoDataListener()
{
}


int CVideoDataListener::init(const QString& strStorage, int iChannel)
{
    m_videoSession.Init(strStorage.toStdString().c_str(), iChannel);
    m_videoSession.GetStreamMgr()->AddStream(this);

    return 1;
}


int CVideoDataListener::setScape(const QDateTime& dtBegin, const QDateTime& dtEnd)
{
    // time format eg:20170527143910-20170528143910
    QString strTime = dtBegin.toString("yyyyMMddhhmmss") + "-" + dtEnd.toString("yyyyMMddhhmmss");
    m_videoSession.SetScape(strTime.toStdString().c_str());

    return 1;
}


int CVideoDataListener::skipTo(const QDateTime& dtPos)
{
    QString strCurrentTime = dtPos.toString("yyyyMMddhhmmss");
    m_videoSession.SkipTo(strCurrentTime.toStdString().c_str());

    return 1;
}


int CVideoDataListener::play()
{
    m_videoSession.Play();
    m_videoSession.SkipTo("20170411131010");

    return 1;
}


int CVideoDataListener::pause()
{
    m_videoSession.Pause();

    return 1;
}

#include "windows.h"

// 对于视频的接接收者，还需要关心这个图像数据的高宽，pCustomData 
// 指向了一个struct{ unsigned int width; unsigned int height; } 类型的数据 
// payload 区别can和video 
// buffer：对于视频接收者，为解码后的yuv420p的数据缓冲区；对于Can数据接收者，为CCanData类型的缓冲区 
// length： 对于视频，为解码后的长度，对于Can数据接收为CCanData的数据数量 
// secs：为距离1970以来的秒数【一个很大的数】，usecs为毫秒数【范围0 - 1000】 
void CVideoDataListener::OnMedia(unsigned char* buffer, unsigned long length, unsigned long payload,
    unsigned long secs, unsigned long msecs, void* pCustomData)
{
    // 1 - video data, 3 - can data 
    if (payload == 1)   // video data 
    {
        imageInfo_t* imageInfo = (imageInfo_t*)pCustomData;

        // yuv -> rgb ????  
        QImage image(buffer, imageInfo->iWidth, imageInfo->iHeight, QImage::Format_RGB32);

        // current time 
        QDateTime currentDateTime = QDateTime::fromMSecsSinceEpoch(secs * 1000 + msecs);

        emit sigVideoUpdate(image, currentDateTime);
    }
    else if (payload == 3)  // can data 
    {
        return;
    }
}

