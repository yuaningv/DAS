#ifndef CCanDataListener_h
#define CCanDataListener_h

#include "QtCore/QObject"
#include "constant.h"
#include "type.h"

#include "CanDecoder.h"
#include "CanFileSession.h"
#include "StreamMgr.h"


class CCanDataListener : public QObject, public CStreamListener
{
    Q_OBJECT

public:
    CCanDataListener(QObject *parent = 0);
    ~CCanDataListener();

public:
    int init(const QString& strStorage, const QString& strProfile, int iChannel);
    int setScape(const QDateTime& dtBegin, const QDateTime& dtEnd);
    int skipTo(const QDateTime& dtPos);
    int play();
    int pause();

protected:
    virtual void OnMedia(unsigned char* buffer, unsigned long length, unsigned long payload,
        unsigned long secs, unsigned long usecs, void* pCustomData);

private:
    CCanDataProfile m_profile;
    CCanFileSession m_canSession;

signals:
    void sigCanUpdate(const QList<CurveLine_t>& lstCanData, const QDateTime& currentDateTime);

private slots:

};


#endif
