#ifndef CGraphicsView_h
#define CGraphicsView_h

#include "QtWidgets/QGraphicsView"
#include "QtCore/QDateTime"
#include "CGraphicsScene.h"
#include "type.h"
#include "constant.h"


class CGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    CGraphicsView(QWidget *parent = 0);
    ~CGraphicsView();

    void setEditModoEnabled(bool enable);
    void saveLayout();

    void setStoragePath(const QString& strPath);

    void setTimeScape(const QDateTime& dtBegin, const QDateTime& dtEnd); 
    QDateTime getDtBegin() { return m_dtBegin; };
    QDateTime getDtEnd() { return m_dtEnd; };

    void skipTo(const QDateTime& currentDateTime);
    void play();
    void pause();

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

    QTimer* m_pTimer;
    int m_iInterval;

    QString m_strStoragePath;

    QDateTime m_dtBegin;
    QDateTime m_dtEnd;

signals:
    void sigItemAttr(const ItemAttribute_t& itemAttr);
    void sigEnd();

private slots:
    void OnUpdateWork();
};


#endif
