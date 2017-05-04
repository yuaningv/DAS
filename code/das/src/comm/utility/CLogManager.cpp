#include "CLogManager.h"

#include "QtCore/QDebug"
#include "QtCore/QCoreApplication"

CLogManager::CGarbo CLogManager::Garbo;
CLogManager* CLogManager::m_pInstance = NULL;
CLogManager::CLogManager(QObject* parent)
:QObject(parent),
CLogThreadCallBack(),
m_bWriteable(false), 
m_logFile0(this),
m_logFile1(this),
m_iCurrentLogFile(0),
m_logThread(this, this)
{
    m_cUTF8[0] = LOG_FILE_UTF8_HEADER1;
    m_cUTF8[1] = LOG_FILE_UTF8_HEADER2;
    m_cUTF8[2] = LOG_FILE_UTF8_HEADER3;
    m_lstLogData.clear();
    m_bStop = false;

    m_pLogOutputCallBack = NULL;
}


CLogManager* CLogManager::getInstance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new CLogManager;
    }

    return m_pInstance;
}


CLogManager::~CLogManager(void)
{
    stopLog();
}


void CLogManager::startLog(const QString& strLogPath)
{
    QString strLogFileName = strLogPath;
    strLogFileName += LOG_FILE_NAME_0;
    m_logFile0.setFileName(strLogFileName);

    strLogFileName = strLogPath;
    strLogFileName += LOG_FILE_NAME_1;
    m_logFile1.setFileName(strLogFileName);

    if (!m_logFile0.open(QIODevice::Append | QIODevice::Text))
    {
        qDebug() << "Create log file 0 failed.";
    }

    if(!m_logFile1.open(QIODevice::Append | QIODevice::Text))
    {
        qDebug() << "Create log file 1 failed.";
    }

    m_bStop = false;
    m_logThread.start();
}

void CLogManager::stopLog()
{
    m_bStop = true;
    m_logThread.wait();
    m_logFile0.close();
    m_logFile1.close();
}


void CLogManager::log(ClientLogLevel_t level, const char* module, const char* format, ...)
{
    va_list args;
    char buffer[1024*10] = {0};
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    LogData_t logData;
    logData.dt = QDateTime::currentDateTime();
    logData.strModule = QString(module);
    logData.thread = QThread::currentThreadId();
    logData.level = level;
    logData.strLog = QString::fromLatin1(buffer);
    QString strLogData = formatLog(logData);
    qDebug() << strLogData;

    m_logWriteLock.lockForWrite();
    m_lstLogData.append(logData);
    m_logWriteLock.unlock();
}


void CLogManager::logA(ClientLogLevel_t level, const char* module, const char* format, ...)
{
    va_list args;
    char buffer[1024*10] = {0};
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    LogData_t logData;
    logData.dt = QDateTime::currentDateTime();
    logData.strModule = QString(module);
    logData.thread = QThread::currentThreadId();
    logData.level = level;
    logData.strLog = QString::fromLatin1(buffer);
    QString strLogData = formatLog(logData);
    qDebug() << strLogData;

    m_logWriteLock.lockForWrite();
    m_lstLogData.append(logData);
    m_logWriteLock.unlock();
}


void CLogManager::log(ClientLogLevel_t level, const char* module,const wchar_t* format, ...)
{
    va_list args;
    wchar_t buffer[1024*10] = {0};
    va_start(args, format);
    vswprintf(buffer, 1024*10, format, args);
    va_end(args);

    LogData_t logData;
    logData.dt = QDateTime::currentDateTime();
    logData.strModule = QString(module);
    logData.thread = QThread::currentThreadId();
    logData.level = level;
    logData.strLog = QString::fromWCharArray(buffer);
    QString strLogData = formatLog(logData);
    qDebug() << strLogData;

    m_logWriteLock.lockForWrite();
    m_lstLogData.append(logData);
    m_logWriteLock.unlock();
}


void CLogManager::logW(ClientLogLevel_t level, const char* module,const wchar_t* format, ...)
{
    va_list args;
    wchar_t buffer[1024*10] = {0};
    va_start(args, format);
    vswprintf(buffer, 1024*10, format, args);
    va_end(args);

    LogData_t logData;
    logData.dt = QDateTime::currentDateTime();
    logData.strModule = QString(module);
    logData.thread = QThread::currentThreadId();
    logData.level = level;
    logData.strLog = QString::fromWCharArray(buffer);
    QString strLogData = formatLog(logData);
    qDebug() << strLogData;

    m_logWriteLock.lockForWrite();
    m_lstLogData.append(logData);
    m_logWriteLock.unlock();
}

void CLogManager::setWriteable(bool bWriteable)
{
    m_bWriteable = bWriteable;
}


bool CLogManager::getWriteable()
{
    return m_bWriteable;
}

void CLogManager::setLogOutputCallBack(CLogOutputCallBack* pLogOutputCallBack)
{
    m_pLogOutputCallBack = pLogOutputCallBack;
}


void CLogManager::writeLog(const QString& strLog)
{
    QFile *pFile = NULL;
    qint64 iSize = 0;

    if(m_iCurrentLogFile == 0)
    {
        iSize = m_logFile0.size();
        if(iSize >= LOG_FILE_MAX_SIZE)      // log0.txt已达到最大长度, 切换到log1.txt
        {
            m_logFile1.resize(0);
            iSize = 0;
            pFile = &m_logFile1;
            m_iCurrentLogFile = 1;
        }
        else
        {
            pFile = &m_logFile0;
        }
    }
    else
    {
        iSize = m_logFile1.size();
        if(iSize >= LOG_FILE_MAX_SIZE)
        {
            m_logFile0.resize(0);
            iSize = 0;
            pFile = &m_logFile0;
            m_iCurrentLogFile = 0;
        }
        else
        {
            pFile = &m_logFile1;
        }
    }

    // 设置文件格式为带BOM utf8格式
    if(iSize == 0)
    {
        pFile->write(m_cUTF8, 3);
    }

    QTextStream out(pFile);
    out.setCodec("UTF-8");
    out << strLog << "\n";
    pFile->flush();
}

QString CLogManager::formatLog(const LogData_t& logData)
{
    QString strLogData = QString("[%1][%2][%3][%4]:%5")
        .arg(logData.dt.toString("yyyy-MM-dd hh:mm:ss zzz"))
        .arg((unsigned int)logData.thread)
        .arg(logData.level)
        .arg(logData.strModule)
        .arg(logData.strLog);

    return strLogData;
}


void CLogManager::logRun(CLogThread* pLogThread)
{
    while(1)
    {
        LogData_t logData;
        if(m_logWriteLock.tryLockForRead(10))
        {
            if(m_lstLogData.size() > 0)
            {
                logData = m_lstLogData.first();
                m_lstLogData.removeFirst();
                m_logWriteLock.unlock();

                if (m_bWriteable)   // 写日志
                {
                    writeLog(formatLog(logData));
                }

                if (m_pLogOutputCallBack != NULL)   // 消息输出栏输出日志
                {
                    m_pLogOutputCallBack->outputLog(logData);
                }
            }
            else
            {
                m_logWriteLock.unlock();
                pLogThread->sleep(30);
            }
        }

        if(m_bStop)         // 全部读取缓存的数据，然后退出
        {
            m_logWriteLock.lockForRead();
            while(m_lstLogData.size() > 0)
            {
                logData = m_lstLogData.first();
                m_lstLogData.removeFirst();

                if(m_bWriteable)
                {
                    writeLog(formatLog(logData));
                }

                if (m_pLogOutputCallBack != NULL)   // 消息输出栏输出日志
                {
                    m_pLogOutputCallBack->outputLog(logData);
                }
            }

            m_logWriteLock.unlock();
            break;
        }
    }
}

