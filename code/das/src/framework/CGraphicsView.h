#ifndef CGraphicsView_h
#define CGraphicsView_h

#include "QtWidgets/QGraphicsView"
#include "CGraphicsScene.h"
#include "type.h"


class CGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    CGraphicsView(QWidget *parent = 0);
    ~CGraphicsView();

    void setEditModoEnabled(bool enable);
    void saveLayout();

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
signals:
    void sigItemAttr(const ItemAttribute_t& itemAttr);
};


#endif
