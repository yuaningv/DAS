#ifndef CGraphicsView_h
#define CGraphicsView_h

#include "QtWidgets/QGraphicsView"
#include "QtCore/QDateTime"
#include "CGraphicsScene.h"
#include "type.h"
#include "constant.h"

#include "VideoFileSession.h"
#include "CanDecoder.h"
#include "CanFileSession.h"


class CGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    CGraphicsView(QWidget *parent = 0);
    ~CGraphicsView();

    void setEditModoEnabled(bool enable);
    void saveLayout();

    void setStoragePath(const QString& strPath);
    QString getStoragePath() const { return m_strStoragePath; }
    void setTimeScape(const QDateTime& dtBegin, const QDateTime& dtEnd); 
    QDateTime getDtBegin() { return m_dtBegin; };
    QDateTime getDtEnd() { return m_dtEnd; };
    void skipTo(const QDateTime& currentDateTime);
    void play();
    void pause();
    void setStep(int iStep);

private:
    void readXml();

protected:
    virtual void dragEnterEvent(QDragEnterEvent * event);
    virtual void dragLeaveEvent(QDragLeaveEvent * event);
    virtual void dragMoveEvent(QDragMoveEvent * event);
    virtual void dropEvent(QDropEvent * event);

    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *ev);
    virtual void keyReleaseEvent(QKeyEvent *ev);

private:
    CGraphicsScene* m_pScene;
    bool m_bEditFlag;

    QString m_strStoragePath;           // storage path 
    QString m_strVideoStorage;          // video storage path 
    QString m_strCanStorage;            // can storage path 
    QDateTime m_dtBegin;                // begin time 
    QDateTime m_dtEnd;                  // end time 
    QDateTime m_dtSkip;                 // skip time  

    QMap<int, CVideoFileSession*> m_mapVideoSession;         // key - channel, value - video session 
    QMap<int, CCanFileSession*> m_mapCanSession;             // key - channel, value - can session 
    CCanDataProfile m_profile;

signals:
    void sigItemAttr(const ItemAttribute_t& itemAttr);      // test 
    void sigEnd();

private slots:
    void OnChannelChanged(QGraphicsItem* item, const int& iPreChannel);
    void OnSkipTo(const QDateTime& currentDt);
    void OnEnd();
};


#endif
