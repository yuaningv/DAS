#include "CVideoSession.h"

#include "QtCore/QDateTime"


CVideoSession::CVideoSession()
{
}


CVideoSession::~CVideoSession()
{
}


int CVideoSession::init(const QString& strStorage, int iChannel)
{
    m_videoSession.Init(strStorage.toStdString().c_str(), iChannel);
    m_videoSession.GetStreamMgr()->AddStream(this);

    return 1;
}


int CVideoSession::setScape(const QDateTime& dtBegin, const QDateTime& dtEnd)
{
    // time format eg:20170527143910-20170528143910
    QString strTime = dtBegin.toString("yyyyMMddhhmmss") + "-" + dtEnd.toString("yyyyMMddhhmmss");
    m_videoSession.SetScape(strTime.toStdString().c_str());

    return 1;
}


int CVideoSession::skipTo(const QDateTime& dtPos)
{
    QString strCurrentTime = dtPos.toString("yyyyMMddhhmmss");
    m_videoSession.SkipTo(strCurrentTime.toStdString().c_str());

    return 1;
}


int CVideoSession::play()
{
    m_videoSession.Play();
    m_videoSession.SkipTo("20170411131010");

    return 1;
}


int CVideoSession::pause()
{
    m_videoSession.Pause();

    return 1;
}

