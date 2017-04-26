#ifndef CLogThread_h
#define CLogThread_h

#include "QtCore/QThread"

class CLogThread;

class CLogThreadCallBack
{
public:
    CLogThreadCallBack() { };
    virtual ~CLogThreadCallBack() { };

    virtual void logRun(CLogThread* pLogThread) { };
};


class CLogThread : public QThread
{
    Q_OBJECT

public:
    CLogThread(QObject * parent, CLogThreadCallBack* pCallBack);
    ~CLogThread();

    void sleep(long ms);
    int exec();
protected:
    virtual void run();
    CLogThreadCallBack* m_pCallBack;
};


#endif

