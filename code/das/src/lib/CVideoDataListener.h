#ifndef CVideoDataListener_h
#define CVideoDataListener_h

#include "QtCore/QObject"

#include "constant.h"

#include "VideoFileSession.h"
#include "StreamMgr.h"


class CVideoDataListener : public QObject, public CStreamListener
{
    Q_OBJECT

public:
    CVideoDataListener(QObject *parent = 0);
    ~CVideoDataListener();

public:
    int init(const QString& strStorage, int iChannel);
    int setScape(const QDateTime& dtBegin, const QDateTime& dtEnd);
    int skipTo(const QDateTime& dtPos);
    int play();
    int pause();

protected:
    virtual void OnMedia(unsigned char* buffer, unsigned long length, unsigned long payload,
        unsigned long secs, unsigned long usecs, void* pCustomData);

private:
    CVideoFileSession m_videoSession;

signals:
    void sigVideoUpdate(const QImage& image, const QDateTime& currentDateTime);

    private slots:
};


#endif
