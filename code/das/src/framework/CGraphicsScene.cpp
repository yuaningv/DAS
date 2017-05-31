#include "CGraphicsScene.h"
#include "constant.h"
#include "CConfigCurveDlg.h"
#include "CCurveGraphicsItem.h"

#include "QtWidgets/QGraphicsSceneMouseEvent"
#include "QtWidgets/QMenu"
#include "QtWidgets/QGraphicsItem"
#include "QtWidgets/QInputDialog"
#include "QtWidgets/QGraphicsProxyWidget"
#include "CVideoWidget.h"
#include "CTableView.h"
#include "CCurveGraphicsItem.h"

const char* cstRemove = "Remove";
extern const char* cstSelectCurve;
extern const char* cstOk;
extern const char* cstCancel;

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

        QGraphicsProxyWidget* pWidget = nullptr;
        CCustomWidgetBase* pCustomItem = nullptr;
        ITEMTYPE iType = Item_None;
        if (item->isWidget())
        {
            pWidget = (QGraphicsProxyWidget*)item;
            pCustomItem = (CCustomWidgetBase*)pWidget->widget();
            iType = pCustomItem->type();
        }
       
        if (iType != Item_TimeAxis || !item->isWidget())         // time axis Ã»ÓÐchannel
        {
            QAction *propertyAction = menu.addAction(trFormString("Property"));
            connect(propertyAction, &QAction::triggered, [=](){
                QInputDialog oTmpDlg;
                oTmpDlg.setWindowFlags(oTmpDlg.windowFlags() & ~Qt::WindowContextHelpButtonHint);
                oTmpDlg.setIntMinimum(0);
                oTmpDlg.setInputMode(QInputDialog::IntInput);
                oTmpDlg.setWindowTitle(trFormString("Property"));
                oTmpDlg.setLabelText(trFormString("Channel"));
                oTmpDlg.setOkButtonText(trFormString(cstOk));
                oTmpDlg.setCancelButtonText(trFormString(cstCancel));
                if (oTmpDlg.exec())
                {
                    if (item->isWidget())
                    {
                        QGraphicsProxyWidget* pWidget = (QGraphicsProxyWidget*)item;
                        CCustomWidgetBase* pCustomItem = (CCustomWidgetBase*)pWidget->widget();
                        ITEMTYPE iType = pCustomItem->type();
                        if (iType == Item_Video)         // video 
                        {
                            CVideoWidget *pVideoWidget = dynamic_cast<CVideoWidget*>(pCustomItem);
                            pVideoWidget->setChannel(oTmpDlg.intValue());
                        }
                        else if (iType == Item_Table)           // table 
                        {
                            CTableView *pTableView = dynamic_cast<CTableView*>(pCustomItem);
                            pTableView->setChannel(oTmpDlg.intValue());
                        }
                    }
                    else // curve
                    {
                        dynamic_cast<CCurveGraphicsItem*>(item)->setChannel(oTmpDlg.intValue());
                    }
                }
            });
        }
        

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



