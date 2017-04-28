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
#include "CFileOperationManager.h"


CGraphicsView::CGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
    , m_bEditFlag(false)
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

    readXml();
}


CGraphicsView::~CGraphicsView()
{
    if (m_pScene != NULL)
    {
        delete m_pScene;
        m_pScene = NULL;
    }
}

void CGraphicsView::setEditModoEnabled(bool enable)
{
    QList<QGraphicsItem*> lstTmpItems = this->scene()->items();
    for (QGraphicsItem* item : lstTmpItems)
    {
        if (item->isWidget())
        {
            QGraphicsProxyWidget* pTmpWgt = static_cast<QGraphicsProxyWidget*>(item);
            static_cast<CCustomWidgetBase*>(pTmpWgt->widget())->setEditModeEnabled(enable);
        }
        else
        {
            static_cast<CCurveGraphicsItem*>(item)->setEnableEditMode(enable);
        }
    }
    m_bEditFlag = enable; 
}

// 保存布局
void CGraphicsView::saveLayout()
{
    QMap<int, QList<WidgetProperty>> mapTmpItems;
    mapTmpItems.clear();
    QList<QGraphicsItem*> lstTmpItems = this->scene()->items();

    for (QGraphicsItem* item : lstTmpItems)
    {
        WidgetProperty tmpWgtPro;
        if (item->isWidget())
        {
           // QGraphicsProxyWidget* pTmpWgt = static_cast<QGraphicsProxyWidget*>(item);
            //static_cast<CCustomWidgetBase*>(pTmpWgt->widget())->setEditModeEnabled(enable);
        }
        else
        {
            tmpWgtPro.m_type = Item_Chart;
            tmpWgtPro.m_realX = item->mapRectFromScene(item->boundingRect()).topLeft().toPoint().x();
            tmpWgtPro.m_realY = item->mapRectFromScene(item->boundingRect()).topLeft().toPoint().y();
            tmpWgtPro.m_realWidth = item->boundingRect().bottomRight().x() - item->boundingRect().topLeft().x();
            tmpWgtPro.m_realHeight = item->boundingRect().bottomRight().y() - item->boundingRect().topLeft().y();

            mapTmpItems[Item_Chart].append(tmpWgtPro);
        }
    }

    CFileOperationManager cfm("test.xml");
    cfm.writeXmlFile(mapTmpItems);
}

// 读取XML
void CGraphicsView::readXml()
{
    CFileOperationManager cfm("test.xml");
    QMap<int, QList<WidgetProperty>> mapTmpItems;
    if (!cfm.ReadXmlFile(mapTmpItems))
    {
        return;
    }

    for (auto& itr = mapTmpItems.begin(); itr != mapTmpItems.end(); ++itr)
    {
        if (Item_TimeAxis == itr.key())
        {
            for (auto& obj : itr.value())
            {
                CTimeAxis* pTimeAxis = new CTimeAxis;
                pTimeAxis->resize(obj.m_realWidth, obj.m_realHeight);
                pTimeAxis->setTimeRange(obj.m_strStart, obj.m_strEnd);
                pTimeAxis->setSliderPosition(obj.m_strPlayPos);
                m_pScene->addWidget(pTimeAxis);
                pTimeAxis->move(obj.m_realX, obj.m_realY);
            }
        }
        else if (Item_Video == itr.key())
        {
            for (auto& obj : itr.value())
            {
                CVideoWidget* pVideo = new CVideoWidget;
                pVideo->resize(obj.m_realWidth, obj.m_realHeight);
                m_pScene->addWidget(pVideo);
                pVideo->move(obj.m_realX, obj.m_realY);
            }
        }
        else if (Item_Chart == itr.key())
        {
            for (auto& obj : itr.value())
            {
                CCurveGraphicsItem* pItem = new CCurveGraphicsItem;
                QRectF rectF(0, 0, obj.m_realWidth, obj.m_realHeight);
                pItem->resetItemSize(rectF);
                m_pScene->addItem(pItem);
                pItem->moveBy(-obj.m_realX, -obj.m_realY);
            }
        }
        else if (Item_Table == itr.key())
        {
            for (auto& obj : itr.value())
            {
                CTableView* pTable = new CTableView;
                pTable->resize(obj.m_realWidth, obj.m_realHeight);
                m_pScene->addWidget(pTable);
                pTable->move(obj.m_realX, obj.m_realY);
            }
        }
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
        case Item_TimeAxis:  // 时间轴
            qDebug() << "Time Axis";
            pTmpWgt = new CTimeAxis();
            pTmpWgt->setEditModeEnabled(m_bEditFlag);
            m_pScene->addWidget(pTmpWgt);
            pTmpWgt->move(mapToScene(event->pos()).toPoint());
            break;

        case Item_Video:     // 视频窗
            {
                CVideoWidget* pVideoWidget = new CVideoWidget;
                pVideoWidget->setEditModeEnabled(m_bEditFlag);
                QGraphicsProxyWidget*pWidget = m_pScene->addWidget(pVideoWidget);
                pVideoWidget->move(mapToScene(event->pos()).toPoint());
            }
            break;

        case Item_Chart:    // chart
        {
            qDebug() << "chart";
            CCurveGraphicsItem* item = new CCurveGraphicsItem();
            item->setEnableEditMode(m_bEditFlag);
            m_pScene->addItem(item);
            item->moveBy(mapToScene(event->pos()).rx(), mapToScene(event->pos()).ry());
        }
            break;

        case Item_Table:    // table
            qDebug() << "table";
            pTmpTableView = new CTableView();
            pTmpTableView->setEditModeEnabled(m_bEditFlag);
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
    if (pItem->isWidget())
    {
        QGraphicsProxyWidget* pProxyWidget = (QGraphicsProxyWidget*)pItem;
        pProxyWidget->widget()->resize(this->width(), this->height());
    }

    QGraphicsView::mouseDoubleClickEvent(ev);
}


void CGraphicsView::keyReleaseEvent(QKeyEvent *ev)
{
    QGraphicsItem* pItem = this->itemAt(m_mousePos);
    if (pItem->isWidget())
    {
        QGraphicsProxyWidget* pProxyWidget = (QGraphicsProxyWidget*)pItem;

        pProxyWidget->widget()->resize(500, 400);
    }

    QGraphicsView::keyReleaseEvent(ev);
}


