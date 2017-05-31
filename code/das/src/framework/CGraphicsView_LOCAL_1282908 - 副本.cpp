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
    m_strStoragePath(""),
    m_strVideoStorage(""),
    m_strCanStorage("")
{
    setAttribute(Qt::WA_DeleteOnClose);
    setCursor(Qt::ArrowCursor);
    setMouseTracking(false);
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    m_pScene = new CGraphicsScene(parent);

    setScene(m_pScene);
    setSceneRect(QRectF(0, 0, this->width(), this->height()));
    setBackgroundBrush(QBrush(Qt::gray, Qt::SolidPattern));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setRenderHint(QPainter::Antialiasing, true);

    m_dtBegin = QDateTime::fromString("2017/04/11 01:01:01", "yyyy/MM/dd hh:mm:ss");
    m_dtEnd = QDateTime::fromString("2017/05/01 01:01:01", "yyyy/MM/dd hh:mm:ss");

    m_mapVideoSession.clear();
    m_mapCanSession.clear();
    m_profile.Init("./candatadefine.xml");

    readXml();
}


CGraphicsView::~CGraphicsView()
{
    if (m_pScene != NULL)
    {
        delete m_pScene;
        m_pScene = NULL;
    }

    QMap<int, CVideoFileSession*>::iterator iterVideo = m_mapVideoSession.begin();
    for (; iterVideo != m_mapVideoSession.end(); iterVideo++)
    {
        iterVideo.value()->Deinit();
    }

    QMap<int, CCanFileSession*>::iterator iterCan = m_mapCanSession.begin();
    for (; iterCan != m_mapCanSession.end(); iterCan++)
    {
        iterCan.value()->Deinit();
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


void CGraphicsView::setStoragePath(const QString& strPath)
{
    m_strStoragePath = strPath;
    m_strVideoStorage = m_strStoragePath + "/videos";
    m_strCanStorage = m_strStoragePath + "/cans";

    QMap<int, CVideoFileSession*>::iterator iterVideo = m_mapVideoSession.begin();
    for (; iterVideo != m_mapVideoSession.end(); iterVideo++)
    {
        iterVideo.value()->Init(m_strVideoStorage.toStdString().c_str(), iterVideo.key());
    }

    QMap<int, CCanFileSession*>::iterator iterCan = m_mapCanSession.begin();
    for (; iterCan != m_mapCanSession.end(); iterCan++)
    {
        iterCan.value()->Init(m_strCanStorage.toStdString().c_str(), &m_profile, iterCan.key());
    }
}


void CGraphicsView::setTimeScape(const QDateTime& dtBegin, const QDateTime& dtEnd)
{
    m_dtBegin = dtBegin;
    m_dtEnd = dtEnd;

    QString strTime = m_dtBegin.toString("yyyyMMddhhmmss") + "-" + m_dtEnd.toString("yyyyMMddhhmmss");

    QMap<int, CVideoFileSession*>::iterator iterVideo = m_mapVideoSession.begin();
    for (; iterVideo != m_mapVideoSession.end(); iterVideo++)
    {
        iterVideo.value()->SetScape(strTime.toStdString().c_str());
    }

    QMap<int, CCanFileSession*>::iterator iterCan = m_mapCanSession.begin();
    for (; iterCan != m_mapCanSession.end(); iterCan++)
    {
        iterCan.value()->SetScape(strTime.toStdString().c_str());
    }

    // test curve ??? 
    QList<QGraphicsItem*> lstItem = this->items();
    for (int i = 0; i < lstItem.size(); i++)
    {
        QGraphicsItem* pItem = lstItem.at(i);
        if (pItem->isWidget())      // timeaxis、video、table 
        {
        }
        else        // 曲线图 
        {
            CCurveGraphicsItem* pCurveItem = (CCurveGraphicsItem*)pItem;
            double dbMin = QDateTime::fromString("20170412141154", "yyyyMMddhhmmss").toMSecsSinceEpoch();
            double dbMax = QDateTime::fromString("20170412142154", "yyyyMMddhhmmss").toMSecsSinceEpoch();
            pCurveItem->setXAxisRange(dbMin, dbMax);
        }
    }
}


void CGraphicsView::skipTo(const QDateTime& currentDateTime)
{
    m_dtSkip = currentDateTime;
    QString strTime = m_dtSkip.toString("yyyyMMddhhmmss");

    QMap<int, CVideoFileSession*>::iterator iterVideo = m_mapVideoSession.begin();
    for (; iterVideo != m_mapVideoSession.end(); iterVideo++)
    {
        iterVideo.value()->SkipTo(strTime.toStdString().c_str());
    }

    QMap<int, CCanFileSession*>::iterator iterCan = m_mapCanSession.begin();
    for (; iterCan != m_mapCanSession.end(); iterCan++)
    {
        iterCan.value()->SkipTo(strTime.toStdString().c_str());
    }
}


void CGraphicsView::play()
{
    QMap<int, CVideoFileSession*>::iterator iterVideo = m_mapVideoSession.begin();
    for (; iterVideo != m_mapVideoSession.end(); iterVideo++)
    {
        iterVideo.value()->SkipTo("20170411131010");        // test ??? 
        iterVideo.value()->Play();
    }

    QMap<int, CCanFileSession*>::iterator iterCan = m_mapCanSession.begin();
    for (; iterCan != m_mapCanSession.end(); iterCan++)
    {
        iterCan.value()->SkipTo("20170413151254");        // test ??? 
        iterCan.value()->Play();
    }
}


void CGraphicsView::pause()
{
    QMap<int, CVideoFileSession*>::iterator iterVideo = m_mapVideoSession.begin();
    for (; iterVideo != m_mapVideoSession.end(); iterVideo++)
    {
        iterVideo.value()->Pause();
    }

    QMap<int, CCanFileSession*>::iterator iterCan = m_mapCanSession.begin();
    for (; iterCan != m_mapCanSession.end(); iterCan++)
    {
        iterCan.value()->Pause();
    }
}


// 读取XML
void CGraphicsView::readXml()
{
    CFileOperationManager cfm("das.xml");
    QMap<int, QList<WidgetProperty_t> > mapTmpItems;
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
                pVideo->setView(this);
                pVideo->resize(obj.m_realWidth, obj.m_realHeight);
                m_pScene->addWidget(pVideo);
                pVideo->move(-obj.m_realX, -obj.m_realY);

                // 绑定session与listener 
                QMap<int, CVideoFileSession*>::iterator iter = m_mapVideoSession.find(0);    // 0 - channel ??? 
                if (iter != m_mapVideoSession.end() && iter.key() == 0) // 存在 
                {
                    iter.value()->GetStreamMgr()->AddStream(pVideo);
                }
                else // 不存在 
                {
                    CVideoFileSession* pSession = new CVideoFileSession;
                    pSession->Init(m_strVideoStorage.toStdString().c_str(), 0);
                    pSession->GetStreamMgr()->AddStream(pVideo);
                    m_mapVideoSession.insert(0, pSession);
                }
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

                // 绑定session与listener 
                QMap<int, CCanFileSession*>::iterator iter = m_mapCanSession.find(1);    // 1 - channel ??? 
                if (iter != m_mapCanSession.end() && iter.key() == 1)       // 存在，直接绑定 
                {
                    iter.value()->GetStreamMgr()->AddStream(pItem);
                }
                else    // 不存在 
                {
                    CCanFileSession* pSession = new CCanFileSession;
                    pSession->Init(m_strCanStorage.toStdString().c_str(), &m_profile, 1);
                    pSession->GetStreamMgr()->AddStream(pItem);
                    m_mapCanSession.insert(1, pSession);
                }
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

                // 绑定session与listener 
                QMap<int, CCanFileSession*>::iterator iter = m_mapCanSession.find(1);    // 1 - channel ??? 
                if (iter != m_mapCanSession.end() && iter.key() == 1)
                {
                    iter.value()->GetStreamMgr()->AddStream(pTable);
                }
                else    // 不存在 
                {
                    CCanFileSession* pSession = new CCanFileSession;
                    pSession->Init(m_strCanStorage.toStdString().c_str(), &m_profile, 1);
                    pSession->GetStreamMgr()->AddStream(pTable);
                    m_mapCanSession.insert(1, pSession);
                }
            }
        }
    }

    CLogManager::getInstance()->log(eLogInfo, "CGraphicsView::readXml", "read xml success!");
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


