#include "CGraphicsView.h"
#include "type.h"
#include "CTableView.h"
#include "CTimeAxis.h"
#include "CVideoWidget.h"
#include "CCurveGraphicsItem.h"
#include "CLogManager.h"

#include "QtGui/QDropEvent"
#include "QtGui/QDragEnterEvent"
#include "QtGui/QDragLeaveEvent"
#include "QtGui/QDragMoveEvent"
#include "QtCore/QMimeData"
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
    QMap<int, QList<WidgetProperty_t>> mapTmpItems;
    mapTmpItems.clear();
	QList<QGraphicsItem*> lstTmpItems = this->scene()->items(Qt::AscendingOrder);

    for (QGraphicsItem* item : lstTmpItems)
    {
        WidgetProperty_t tmpWgtPro;
        if (item->isWidget())
        {
			QGraphicsProxyWidget* pWidget = (QGraphicsProxyWidget*)item;
			CCustomWidgetBase* pCustomItem = (CCustomWidgetBase*)pWidget->widget();
			ITEMTYPE iType = pCustomItem->type();
			if (iType == Item_Video)
			{
				tmpWgtPro.m_type = Item_Video;
				tmpWgtPro.m_realX = item->mapRectFromScene(item->boundingRect()).topLeft().toPoint().x();
				tmpWgtPro.m_realY = item->mapRectFromScene(item->boundingRect()).topLeft().toPoint().y();
				tmpWgtPro.m_realWidth = item->boundingRect().bottomRight().x() - item->boundingRect().topLeft().x();
				tmpWgtPro.m_realHeight = item->boundingRect().bottomRight().y() - item->boundingRect().topLeft().y();

				//mapTmpItems[Item_Video].append(tmpWgtPro);
			}
			else if (iType == Item_TimeAxis)
			{
				tmpWgtPro.m_type = Item_TimeAxis;
				tmpWgtPro.m_realX = item->mapRectFromScene(item->boundingRect()).topLeft().toPoint().x();
				tmpWgtPro.m_realY = item->mapRectFromScene(item->boundingRect()).topLeft().toPoint().y();
				tmpWgtPro.m_realWidth = item->boundingRect().bottomRight().x() - item->boundingRect().topLeft().x();
				tmpWgtPro.m_realHeight = item->boundingRect().bottomRight().y() - item->boundingRect().topLeft().y();
				tmpWgtPro.m_strStart = static_cast<CTimeAxis*>(pCustomItem)->getStartTime();
				tmpWgtPro.m_strEnd = static_cast<CTimeAxis*>(pCustomItem)->getEndTime();
				tmpWgtPro.m_strPlayPos = static_cast<CTimeAxis*>(pCustomItem)->getSliderPosition();

				//mapTmpItems[Item_TimeAxis].append(tmpWgtPro);
			}
			else if (iType == Item_Table)
			{
				tmpWgtPro.m_type = Item_Table;
				tmpWgtPro.m_realX = item->mapRectFromScene(item->boundingRect()).topLeft().toPoint().x();
				tmpWgtPro.m_realY = item->mapRectFromScene(item->boundingRect()).topLeft().toPoint().y();
				tmpWgtPro.m_realWidth = item->boundingRect().bottomRight().x() - item->boundingRect().topLeft().x();
				tmpWgtPro.m_realHeight = item->boundingRect().bottomRight().y() - item->boundingRect().topLeft().y();

				//mapTmpItems[Item_Table].append(tmpWgtPro);
			}
        }
        else
        {
            tmpWgtPro.m_type = Item_Chart;
            tmpWgtPro.m_realX = item->mapRectFromScene(item->boundingRect()).topLeft().toPoint().x();
            tmpWgtPro.m_realY = item->mapRectFromScene(item->boundingRect()).topLeft().toPoint().y();
            tmpWgtPro.m_realWidth = item->boundingRect().bottomRight().x() - item->boundingRect().topLeft().x();
            tmpWgtPro.m_realHeight = item->boundingRect().bottomRight().y() - item->boundingRect().topLeft().y();

            //mapTmpItems[Item_Chart].append(tmpWgtPro);
        }
		mapTmpItems[tmpWgtPro.m_type].append(tmpWgtPro);
    }

    CFileOperationManager cfm("das.xml");
	if (cfm.writeXmlFile(mapTmpItems))
	{
		CLogManager::getInstance()->log(eLogInfo, "CGraphicsView::saveLayout", "save laout success!");
	}
	else
	{
		CLogManager::getInstance()->log(eLogInfo, "CGraphicsView::saveLayout", "save laout failed!");
	}
}

// 读取XML
void CGraphicsView::readXml()
{
    CFileOperationManager cfm("das.xml");
    QMap<int, QList<WidgetProperty_t>> mapTmpItems;
    if (!cfm.ReadXmlFile(mapTmpItems))
    {
		CLogManager::getInstance()->log(eLogInfo, "CGraphicsView::readXml", "read xml failed!");
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
                pTimeAxis->move(-obj.m_realX, -obj.m_realY);
            }
        }
        else if (Item_Video == itr.key())
        {
            for (auto& obj : itr.value())
            {
                CVideoWidget* pVideo = new CVideoWidget;
                pVideo->resize(obj.m_realWidth, obj.m_realHeight);
                m_pScene->addWidget(pVideo);
                pVideo->move(-obj.m_realX, -obj.m_realY);
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
                pTable->move(-obj.m_realX, -obj.m_realY);
            }
        }
    }
	CLogManager::getInstance()->log(eLogInfo, "CGraphicsView::readXml", "read xml success!");
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
            pTmpWgt = new CTimeAxis;
            pTmpWgt->setEditModeEnabled(m_bEditFlag);
            m_pScene->addWidget(pTmpWgt);
            pTmpWgt->move(mapToScene(event->pos()).toPoint());

            CLogManager::getInstance()->log(eLogDebug, "CGraphicsView", "add time axis");
            break;

        case Item_Video:     // 视频窗
            {
                CVideoWidget* pVideoWidget = new CVideoWidget;
                pVideoWidget->setView(this);
                pVideoWidget->setEditModeEnabled(m_bEditFlag);
                QGraphicsProxyWidget*pWidget = m_pScene->addWidget(pVideoWidget);
                pVideoWidget->move(mapToScene(event->pos()).toPoint());
            }
            break;

        case Item_Chart:    // chart
        {
            CCurveGraphicsItem* item = new CCurveGraphicsItem();
            item->setEnableEditMode(m_bEditFlag);
            m_pScene->addItem(item);
            item->moveBy(mapToScene(event->pos()).rx(), mapToScene(event->pos()).ry());

            CLogManager::getInstance()->log(eLogDebug, "CGraphicsView", "add curve chart");
        }
            break;

        case Item_Table:    // table
            pTmpTableView = new CTableView();
            pTmpTableView->setEditModeEnabled(m_bEditFlag);
            m_pScene->addWidget(pTmpTableView);
            pTmpTableView->move(mapToScene(event->pos()).toPoint());

            CLogManager::getInstance()->log(eLogDebug, "CGraphicsView", "add table");
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


void CGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsItem* pItem = this->itemAt(event->pos());
    if (pItem != NULL)
    {
        if (pItem->isWidget())
        {
            QGraphicsProxyWidget* pWidget = (QGraphicsProxyWidget*)pItem;
            pWidget->boundingRect();
            CCustomWidgetBase* pCustomItem = (CCustomWidgetBase*)pWidget->widget();
            ITEMTYPE iType = pCustomItem->type();
            if (iType == Item_Video)
            {
                CVideoWidget *pVideoWidget = dynamic_cast<CVideoWidget*>(pCustomItem);
                ItemAttribute_t itemAttr;
                itemAttr.iID = pVideoWidget->getID();
                itemAttr.iType = iType;
                itemAttr.strTitle = pVideoWidget->getTitle();
                itemAttr.iChannel = 1;
                itemAttr.iPosX = pVideoWidget->pos().x();//pVideoWidget->pos().x();
                itemAttr.iPoxY = pVideoWidget->pos().y();
                itemAttr.iWidth = pVideoWidget->width();
                itemAttr.iHeight = pVideoWidget->height();

                emit sigItemAttr(itemAttr);
            }
            else if (iType == Item_Table)
            {

                CTableView *pTable = dynamic_cast<CTableView*>(pCustomItem);
            }
            else if (iType == Item_TimeAxis)
            {

                CTimeAxis *pTimeAxis = dynamic_cast<CTimeAxis*>(pCustomItem);
            }
        }
        else
        {
            CCurveGraphicsItem* pCurveItem = (CCurveGraphicsItem*)pItem;
        }
    }

    QGraphicsView::mouseReleaseEvent(event);
}


void CGraphicsView::mouseMoveEvent(QMouseEvent *event)
{

    QGraphicsView::mouseMoveEvent(event);
}


void CGraphicsView::mouseDoubleClickEvent(QMouseEvent *ev)
{
    QGraphicsView::mouseDoubleClickEvent(ev);
}


void CGraphicsView::keyReleaseEvent(QKeyEvent *ev)
{
    QGraphicsView::keyReleaseEvent(ev);
}


