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
#include "QtCore/QTimer"


CGraphicsView::CGraphicsView(QWidget *parent)
    : QGraphicsView(parent),
    m_bEditFlag(false),
    m_iInterval(1000)
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

    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(m_iInterval);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(OnUpdateWork()));
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


void CGraphicsView::play()
{
    m_pTimer->start();
}


void CGraphicsView::pause()
{
    m_pTimer->stop();
}


void CGraphicsView::OnUpdateWork()
{
    QList<QGraphicsItem*> lstItem = this->items();
    for (int i = 0; i < lstItem.size(); i++)
    {
        QGraphicsItem* pItem = lstItem.at(i);
        if (pItem->isWidget())      // timeaxis、video、table 
        {
            QGraphicsProxyWidget* pWidget = (QGraphicsProxyWidget*)pItem;
            CCustomWidgetBase* pCustomItem = (CCustomWidgetBase*)pWidget->widget();
            ITEMTYPE iType = pCustomItem->type();
            if (iType == Item_TimeAxis)         // time axis 
            {
                CTimeAxis *pTimeAxis = dynamic_cast<CTimeAxis*>(pCustomItem);

                if (pTimeAxis->getValue() >= pTimeAxis->getEndTime())
                {
                    m_pTimer->stop();
                    pTimeAxis->setValue(pTimeAxis->getStartTime());
                    emit sigEnd();
                    return;
                }

                QDateTime dt = QDateTime::fromString(pTimeAxis->getValue(), "yyyy/MM/dd hh:mm:ss:zzz");
                QString dtCurrent = dt.toString("yyyy/MM/dd hh:mm:ss:zzz");
                QString strValue = dt.addMSecs(m_iInterval).toString("yyyy/MM/dd hh:mm:ss:zzz");
                pTimeAxis->setValue(strValue);
            }
            else if (iType == Item_Video)         // video 
            {
                CVideoWidget *pVideoWidget = dynamic_cast<CVideoWidget*>(pCustomItem);

            }
            else if (iType == Item_Table)
            {
                // 接收回调数据
                QList<CurveLine_t> lstTmpData;

                CTableView *pTable = dynamic_cast<CTableView*>(pCustomItem);
                pTable->insertRowData(lstTmpData);
            }
        }
        else        // 曲线图 
        {
            CCurveGraphicsItem* pCurveItem = (CCurveGraphicsItem*)pItem;

            QList<CurveLine_t> lstTmpLines = pCurveItem->getLines();
            // 接收数据点
            for (auto& TmpLine : lstTmpLines)
            {
                if (TmpLine.m_strName == "")  // 判断回调数据是哪条线
                {
                    if (lstTmpLines.size() > 1)
                    {
                        TmpLine.m_vecPoints.append(TmpLine.m_vecPoints.last());
                    }
                    TmpLine.m_vecPoints.append(QPointF());  // 当前点记录起来
                }
            }

            // 更新曲线图
            pCurveItem->setLines(lstTmpLines);
        }
    }
}


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
                tmpWgtPro.m_strPlayPos = static_cast<CTimeAxis*>(pCustomItem)->getValue();

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
                pTimeAxis->setValue(obj.m_strPlayPos);
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
        switch (itemType)
        {
        case Item_TimeAxis:  // 时间轴
            {
                CTimeAxis* pTimeAxis = new CTimeAxis;
                pTimeAxis->setEditModeEnabled(m_bEditFlag);
                m_pScene->addWidget(pTimeAxis);
                pTimeAxis->move(mapToScene(event->pos()).toPoint());

                CLogManager::getInstance()->log(eLogDebug, "CGraphicsView", "add time axis");
            }
            break;

        case Item_Video:     // 视频窗
            {
                CVideoWidget* pVideoWidget = new CVideoWidget;
                pVideoWidget->setView(this);
                pVideoWidget->setEditModeEnabled(m_bEditFlag);
                m_pScene->addWidget(pVideoWidget);
                pVideoWidget->move(mapToScene(event->pos()).toPoint());
            }
            break;

        case Item_Chart:    // chart
            {
                CCurveGraphicsItem* pCurveItem = new CCurveGraphicsItem();
                pCurveItem->setEnableEditMode(m_bEditFlag);
                m_pScene->addItem(pCurveItem);
                pCurveItem->moveBy(mapToScene(event->pos()).rx(), mapToScene(event->pos()).ry());

                CLogManager::getInstance()->log(eLogDebug, "CGraphicsView", "add curve chart");
            }
            break;

        case Item_Table:    // table
            {
                CTableView* pTableView = new CTableView();
                pTableView->setEditModeEnabled(m_bEditFlag);
                m_pScene->addWidget(pTableView);
                pTableView->move(mapToScene(event->pos()).toPoint());

                CLogManager::getInstance()->log(eLogDebug, "CGraphicsView", "add table");
            }
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
        if (pItem->isWidget())      // 进度条、视频、表格 
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
        else        // 曲线图 
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


