#ifndef CLogManager_h
#define CLogManager_h

#include "QtCore/QObject"
#include "QtCore/QDateTime"
#include "QtCore/QFile"
#include "QtCore/QReadWriteLock"

#include "CLogThread.h"


typedef enum ClientLogLevel
{
    eLogNone = 0,
    eLogDebug,
    eLogInfo,
    eLogWarning,
    eLogError
} ClientLogLevel_t;


typedef struct LogData 
{
    QDateTime dt;
    QString strModule;
    Qt::HANDLE thread;
    ClientLogLevel_t level;
    QString strLog;

    LogData() 
    {
        dt = QDateTime::currentDateTime();
        strModule = "";
        thread = 0;
        level = eLogNone;
        strLog = "";
    }
} LogData_t;

#define LOG_FILE_MAX_SIZE 10*1024*1024
#define LOG_FILE_NAME_0 "/log0.txt"
#define LOG_FILE_NAME_1 "/log1.txt"
#define LOG_FILE_UTF8_HEADER1 0xEF
#define LOG_FILE_UTF8_HEADER2 0xBB
#define LOG_FILE_UTF8_HEADER3 0xBF


// 消息输出栏，回调类
class CLogOutputCallBack
{
public:
    CLogOutputCallBack() { };
    virtual ~CLogOutputCallBack() { };

    virtual void outputLog(const LogData_t& logData) { Q_UNUSED(logData); };
};


class CLogManager : public QObject, public CLogThreadCallBack
{
    Q_OBJECT

private:
    CLogManager(QObject* parent = 0);
    ~CLogManager(void);
public:
    static CLogManager* getInstance();

private:
    class CGarbo
    {
    public:
        CGarbo(){}
        ~CGarbo()
        {
            if (CLogManager::m_pInstance)
            {
                delete CLogManager::m_pInstance;
            }
        }
    };
    static CGarbo Garbo;

protected:
    virtual void logRun(CLogThread* pLogThread);

public:
    void startLog(const QString& strLogPath);
    void stopLog();

    // 标准字节
    void log(ClientLogLevel_t level, const char* module, const char* format, ...);
    void logA(ClientLogLevel_t level, const char* module, const char* format, ...);

    // 宽字节支持
    void log(ClientLogLevel_t level, const char* module, const wchar_t* format, ...);
    void logW(ClientLogLevel_t level, const char* module, const wchar_t* format, ...);

    void setWriteable(bool bWriteable);
    bool getWriteable();

    void setLogOutputCallBack(CLogOutputCallBack* pLogOutputCallBack);

private:
    void writeLog(const QString& strLog);
    QString formatLog(const LogData_t& logData);

private:
    static CLogManager* m_pInstance;
    bool m_bWriteable;
    QFile m_logFile0;
    QFile m_logFile1;
    int m_iCurrentLogFile;
    char m_cUTF8[3];
    bool m_bStop;
    CLogThread m_logThread;
    QReadWriteLock m_logWriteLock;
    QList<LogData_t> m_lstLogData;
    CLogOutputCallBack* m_pLogOutputCallBack;
};


#endif

