#include "CGraphicsScene.h"
#include "constant.h"
#include "CConfigCurveDlg.h"
#include "CCurveGraphicsItem.h"

#include "QtWidgets/QGraphicsSceneMouseEvent"
#include "QtWidgets/QMenu"
#include "QtWidgets/QGraphicsItem"

const char* cstRemove = "Remove";
extern const char* cstSelectCurve;

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
    QGraphicsItem* item = this->itemAt(event->scenePos(), QTransform());

    if (item)
    {
        // ÓÒ¼üÒÆ³ýitem
        QMenu menu;
        QAction *removeAction = menu.addAction(trFormString(cstRemove));
        connect(removeAction, &QAction::triggered, [=](){this->removeItem(item);});

        if (!item->isWidget())
        {
            QAction *curveAction = menu.addAction(trFormString(cstSelectCurve));
            connect(curveAction, &QAction::triggered, [=](){
                CConfigCurveDlg TmpObj;
                TmpObj.setCheckedLines(static_cast<CCurveGraphicsItem*>(item)->getLines());
                if (TmpObj.exec())
                {
                    static_cast<CCurveGraphicsItem*>(item)->setLines(TmpObj.getCheckedLines());
                }
            });
        }
        QAction *selectedAction = menu.exec(event->screenPos());
    }

    QGraphicsScene::contextMenuEvent(event);
}



