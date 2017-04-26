#include "CGraphicsView.h"
#include "type.h"
#include "CTableView.h"
#include "CTimeAxis.h"

#include "QtGui/QDropEvent"
#include "QtGui/QDragEnterEvent"
#include "QtGui/QDragLeaveEvent"
#include "QtGui/QDragMoveEvent"
#include "QtCore/QMimeData"
#include "QtCore/QDebug"


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
    //setBackgroundBrush(QBrush(Qt::gray, Qt::Dense7Pattern));
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
            pTmpWgt->move(event->pos());
            break;

        case Item_Video:     //  ”∆µ¥∞
            qDebug() << "video";
            break;

        case Item_Chart:    // chart
            qDebug() << "chart";
            break;

        case Item_Table:    // table
            qDebug() << "table";
            pTmpTableView = new CTableView();
            m_pScene->addWidget(pTmpTableView);
            pTmpTableView->move(event->pos());
            break;

        default:
            break;
        }
    }
    else
    {
        QWidget::dropEvent(event);
    }
}




