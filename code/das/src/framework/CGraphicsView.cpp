#include "CGraphicsView.h"
#include "type.h"
#include "CTableView.h"
#include "CTimeAxis.h"
#include "CVideoWidget.h"
#include "CCurveGraphicsItem.h"

#include "QtGui/QDropEvent"
#include "QtGui/QDragEnterEvent"
#include "QtGui/QDragLeaveEvent"
#include "QtGui/QDragMoveEvent"
#include "QtCore/QMimeData"
#include "QtCore/QDebug"
#include "QtWidgets/QGraphicsProxyWidget"


CGraphicsView::CGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setCursor(Qt::ArrowCursor);
    setMouseTracking(false);
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    m_pScene = new CGraphicsScene(parent);

    setScene(m_pScene);
    setSceneRect(QRectF(0, 0, this->width(), this->height()));
    setBackgroundBrush(QBrush(Qt::gray, Qt::SolidPattern));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setRenderHint(QPainter::Antialiasing, true);
}


CGraphicsView::~CGraphicsView()
{
    if (m_pScene != NULL)
    {
        delete m_pScene;
        m_pScene = NULL;
    }
}


void CGraphicsView::dragEnterEvent(QDragEnterEvent * event)
{
    if (event->mimeData()->hasFormat("CustomWidget"))
    {
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
        QGraphicsView::dragEnterEvent(event);
    }
}


void CGraphicsView::dragLeaveEvent(QDragLeaveEvent * event)
{
    QGraphicsView::dragLeaveEvent(event);
}


void CGraphicsView::dragMoveEvent(QDragMoveEvent * event)
{
    if (event->mimeData()->hasFormat("CustomWidget"))
    {
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
        QGraphicsView::dragMoveEvent(event);
    }
}


void CGraphicsView::dropEvent(QDropEvent * event)
{
    if (event->mimeData()->hasFormat("CustomWidget"))
    {
        QByteArray mimeData = event->mimeData()->data("CustomWidget");
        QDataStream out(&mimeData, QIODevice::ReadOnly);
        int itemType;
        out >> itemType;
        CTimeAxis* pTmpWgt;
        CTableView* pTmpTableView;
        switch (itemType)
        {
        case Item_TimeAxis:  //  ±º‰÷·
            qDebug() << "Time Axis";
            pTmpWgt = new CTimeAxis();
            m_pScene->addWidget(pTmpWgt);
            pTmpWgt->move(mapToScene(event->pos()).toPoint());
            break;

        case Item_Video:     //  ”∆µ¥∞
            {
                CVideoWidget* pVideoWidget = new CVideoWidget;
                QGraphicsProxyWidget*pWidget = m_pScene->addWidget(pVideoWidget);
                pVideoWidget->move(event->pos());
            }
            break;

        case Item_Chart:    // chart
        {
            qDebug() << "chart";
            CCurveGraphicsItem* item = new CCurveGraphicsItem();
            m_pScene->addItem(item);
            item->moveBy(mapToScene(event->pos()).rx(), mapToScene(event->pos()).ry());
        }
            break;

        case Item_Table:    // table
            qDebug() << "table";
            pTmpTableView = new CTableView();
            m_pScene->addWidget(pTmpTableView);
            pTmpTableView->move(mapToScene(event->pos()).toPoint());
            break;

        default:
            break;
        }
    }
    else
    {
        QGraphicsView::dropEvent(event);
    }
}


void CGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    m_mousePos = event->pos();

    QGraphicsView::mouseMoveEvent(event);
}


void CGraphicsView::mouseDoubleClickEvent(QMouseEvent *ev)
{
    QGraphicsItem* pItem = this->itemAt(ev->pos());
    QGraphicsProxyWidget* pProxyWidget = (QGraphicsProxyWidget*)pItem;
    if (pProxyWidget != NULL)
    {
        pProxyWidget->widget()->resize(this->width(), this->height());
    }

    QGraphicsView::mouseDoubleClickEvent(ev);
}


void CGraphicsView::keyReleaseEvent(QKeyEvent *ev)
{
    QGraphicsItem* pItem = this->itemAt(m_mousePos);
    QGraphicsProxyWidget* pProxyWidget = (QGraphicsProxyWidget*)pItem;
    if (pProxyWidget != NULL)
    {
        pProxyWidget->widget()->resize(500, 400);
    }

    QGraphicsView::keyReleaseEvent(ev);
}


