#ifndef CExportCanData_h
#define CExportCanData_h

#include "QtCore/QObject"
#include "QtWidgets/QProgressDialog"
#include "QtCore/QDateTime"
#include "QtCore/QTimer"

#include "constant.h"
#include "StreamMgr.h"
#include "CanDecoder.h"
#include "CanFileSession.h"
#include "type.h"

#include "QtXlsx/QtXlsx.h"


//#define EXPORT_TEST 


class CExportCanData : public QObject, public CStreamListener
{
    Q_OBJECT

public:
    CExportCanData();
    ~CExportCanData();

    void setTimeRange(const QDateTime& dtStart, const QDateTime& dtEnd);

    // 通过channel来判断该组件是否已经关联数据，iChannel < 0无关联数据，iChannel >= 0关联数据 
    void setChannel(int iChannel);
    int getChannel() { return m_iChannel; };
    void setInitData(int iChannel, const QString& strCanStoragePath);
    void play();
    void setExcelFileName(const QString& strExcelFileName);

protected:
    virtual void OnMedia(unsigned char* buffer, unsigned long length, unsigned long payload,
        CCustomDateTime* pTime, void* pCustomData);

private:
    QDateTime m_dtStartTime;
    QDateTime m_dtEndTime;
    QDateTime m_dtCurrentTime;
    int m_iChannel;
    QString m_strCanStoragePath;
    QString m_strExcelFileName;
    QMutex m_mutex;

    CCanFileExportSession* m_pCanDataExportSession;
    CCanDataProfile m_profile;
    QList<CurveLine_t> m_lstHeaderData;

    QProgressDialog m_progressDialog;

    QXlsx::Document* m_xlsx;
    QXlsx::Format m_xlsxFormat;

#ifdef EXPORT_TEST
    int m_iProgressTest;
    QTimer* m_pTimer;
#endif

private slots:
    void OnExport(const QList<CurveLine_t>& lstCanData);
    void OnCancel();

#ifdef EXPORT_TEST
    void OnTimer();
#endif

signals:
    void sigCanDataUpdate(const QList<CurveLine_t>& lstCanData);
};

#endif // CExportCanData_h
