#ifndef CGraphicsScene_h
#define CGraphicsScene_h

#include "QtWidgets/QGraphicsScene"


class CGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    CGraphicsScene(QObject *parent = 0);
    ~CGraphicsScene();

    void setRightMenuEnable(bool bEnable) { m_bEnable = bEnable; }
    void setPlayFlag(bool bFlag) { m_bPlay = bFlag; }

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
    int getItemProperty(QGraphicsItem* item, QString& strName) const;

private:
    bool m_bEnable;
    bool m_bPlay;

private slots :

signals :
        void sigChannelChanged(QGraphicsItem* item, const int& iPreChannel);
        void sigChanged();
};


#endif
