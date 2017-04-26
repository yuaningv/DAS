#include "CLogThread.h"


CLogThread::CLogThread(QObject * parent, CLogThreadCallBack* pCallBack)
:QThread(parent)
,m_pCallBack(pCallBack)
{
}

CLogThread::~CLogThread()
{
}


void CLogThread::sleep(long ms)
{
    QThread::usleep(ms*1000);
}


int CLogThread::exec()
{
    return QThread::exec();
}


void CLogThread::run()
{
    if(m_pCallBack)
    {
        m_pCallBack->logRun(this);
    }
}


