#include "CGraphicsScene.h"
#include "QtWidgets/QGraphicsSceneMouseEvent"
#include "QtWidgets/QMenu"

CGraphicsScene::CGraphicsScene(QObject *parent)
: QGraphicsScene(parent)
{

}


CGraphicsScene::~CGraphicsScene()
{
}


void CGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
    {
        return;
    }


    QGraphicsScene::mousePressEvent(mouseEvent);
}

void CGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void CGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}


void CGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}


void CGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QGraphicsItem* item = this->focusItem();
    if (item)
    {
        // ÓÒ¼üÒÆ³ýitem
        QMenu menu;
        QAction *removeAction = menu.addAction(tr("Remove"));
        connect(removeAction, &QAction::triggered, [=](){this->removeItem(item);});
        QAction *selectedAction = menu.exec(event->screenPos());
    }

    QGraphicsScene::contextMenuEvent(event);
}



