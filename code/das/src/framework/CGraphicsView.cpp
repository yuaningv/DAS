#include "CGraphicsView.h"
#include "type.h"
#include "CTableView.h"
#include "CTimeAxis.h"
#include "CVideoWidget.h"
#include "CCurveGraphicsItem.h"
#include "CLogManager.h"
#include "CFileOperationManager.h"

#include "QtGui/QDropEvent"
#include "QtGui/QDragEnterEvent"
#include "QtGui/QDragLeaveEvent"
#include "QtGui/QDragMoveEvent"
#include "QtCore/QMimeData"
#include "QtWidgets/QGraphicsProxyWidget"
#include "QtWidgets/QMessageBox"


extern const char* cstDictDas;


CGraphicsView::CGraphicsView(QWidget *parent)
    : QGraphicsView(parent),
    m_bEditFlag(false),
    m_strStoragePath("./"),
    m_strVideoStorage(""),
    m_strCanStorage("")
{
    setAttribute(Qt::WA_DeleteOnClose);
    setCursor(Qt::ArrowCursor);
    setMouseTracking(false);
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    m_pScene = new CGraphicsScene(parent);
    m_pScene->setSceneRect(QRectF(0, 0, this->width(), this->height()));
    connect(m_pScene, &CGraphicsScene::sigChannelChanged, this, &CGraphicsView::OnChannelChanged);
    connect(m_pScene, &CGraphicsScene::sigChanged, [=]{ this->saveLayout(); });

    setScene(m_pScene);
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
    QMap<int, CVideoFileSession*>::iterator iterVideo = m_mapVideoSession.begin();
    for (; iterVideo != m_mapVideoSession.end(); iterVideo++)
    {
        iterVideo.value()->Pause();
        iterVideo.value()->Deinit();
    }

    QMap<int, CCanFileSession*>::iterator iterCan = m_mapCanSession.begin();
    for (; iterCan != m_mapCanSession.end(); iterCan++)
    {
        iterCan.value()->Pause();
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
    m_pScene->setRightMenuEnable(enable);
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

    saveLayout();
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

    QList<QGraphicsItem*> lstItem = this->items();
    for (int i = 0; i < lstItem.size(); i++)
    {
        QGraphicsItem* pItem = lstItem.at(i);
        if (pItem->isWidget())      // timeaxis、video、table 
        {
            // update time axis 
            QGraphicsProxyWidget* pWidget = (QGraphicsProxyWidget*)pItem;
            CCustomWidgetBase* pCustomItem = (CCustomWidgetBase*)pWidget->widget();
            ITEMTYPE iType = pCustomItem->type();
            if (iType == Item_TimeAxis)         // time axis 
            {
                CTimeAxis *pTimeAxis = dynamic_cast<CTimeAxis*>(pCustomItem);
                pTimeAxis->setTimeRange(m_dtBegin.toString("yyyy/MM/dd hh:mm:ss:zzz"), m_dtEnd.toString("yyyy/MM/dd hh:mm:ss:zzz"));
            }
        }
        else        // 曲线图 
        {
            CCurveGraphicsItem* pCurveItem = (CCurveGraphicsItem*)pItem;
            double dbMin = m_dtBegin.toMSecsSinceEpoch();
            double dbMax = m_dtEnd.toMSecsSinceEpoch();
            pCurveItem->setXAxisRange(dbMin, dbMax);
        }
    }

    saveLayout();

    // 每次设置范围重新调用skipto 
    skipTo(m_dtBegin);
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
    m_pScene->setPlayFlag(true);

    QMap<int, CVideoFileSession*>::iterator iterVideo = m_mapVideoSession.begin();
    for (; iterVideo != m_mapVideoSession.end(); iterVideo++)
    {
        iterVideo.value()->Play();
    }

    QMap<int, CCanFileSession*>::iterator iterCan = m_mapCanSession.begin();
    for (; iterCan != m_mapCanSession.end(); iterCan++)
    {
        //iterCan.value()->SkipTo("20170411131010");
        iterCan.value()->Play();
    }

    // 更新时间轴 
    QList<QGraphicsItem*> lstItem = this->items();
    for (int i = 0; i < lstItem.size(); i++)
    {
        QGraphicsItem* pItem = lstItem.at(i);
        if (pItem->isWidget())      // timeaxis、video、table 
        {
            // update time axis 
            QGraphicsProxyWidget* pWidget = (QGraphicsProxyWidget*)pItem;
            CCustomWidgetBase* pCustomItem = (CCustomWidgetBase*)pWidget->widget();
            ITEMTYPE iType = pCustomItem->type();
            if (iType == Item_TimeAxis)         // time axis 
            {
                CTimeAxis *pTimeAxis = dynamic_cast<CTimeAxis*>(pCustomItem);
                pTimeAxis->play();
                break;      // 时间轴只有一个 
            }
        }
    }
}


void CGraphicsView::pause()
{
    m_pScene->setPlayFlag(false);

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

    // 更新时间轴 
    QList<QGraphicsItem*> lstItem = this->items();
    for (int i = 0; i < lstItem.size(); i++)
    {
        QGraphicsItem* pItem = lstItem.at(i);
        if (pItem->isWidget())      // timeaxis、video、table 
        {
            // update time axis 
            QGraphicsProxyWidget* pWidget = (QGraphicsProxyWidget*)pItem;
            CCustomWidgetBase* pCustomItem = (CCustomWidgetBase*)pWidget->widget();
            ITEMTYPE iType = pCustomItem->type();
            if (iType == Item_TimeAxis)         // time axis 
            {
                CTimeAxis *pTimeAxis = dynamic_cast<CTimeAxis*>(pCustomItem);
                pTimeAxis->pause();
                break;      // 时间轴只有一个 
            }
        }
    }
}


void CGraphicsView::setStep(int iStep)
{
    QMap<int, CVideoFileSession*>::iterator iterVideo = m_mapVideoSession.begin();
    for (; iterVideo != m_mapVideoSession.end(); iterVideo++)
    {
        iterVideo.value()->SetStep(iStep);
    }

    QMap<int, CCanFileSession*>::iterator iterCan = m_mapCanSession.begin();
    for (; iterCan != m_mapCanSession.end(); iterCan++)
    {
        iterCan.value()->SetStep(iStep);
    }
}


// 读取XML
void CGraphicsView::readXml()
{
    CFileOperationManager cfm("das.xml");
    QMap<int, QList<WidgetProperty_t> > mapTmpItems;
    QString strTimeBegin, strTimeEnd;
    if (!cfm.ReadXmlFile(mapTmpItems, m_strStoragePath, strTimeBegin, strTimeEnd))
    {
        CLogManager::getInstance()->log(eLogInfo, "CGraphicsView::readXml", "read xml failed!");
        return;
    }

    m_strVideoStorage = m_strStoragePath + "/videos";
    m_strCanStorage = m_strStoragePath + "/cans";
    m_dtBegin = QDateTime::fromString(strTimeBegin, "yyyy/MM/dd hh:mm:ss:zzz");
    m_dtEnd = QDateTime::fromString(strTimeEnd, "yyyy/MM/dd hh:mm:ss:zzz");

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
                connect(pTimeAxis, SIGNAL(sigSkipTo(const QDateTime&)), this, SLOT(OnSkipTo(const QDateTime&)));
                connect(pTimeAxis, SIGNAL(sigEnd()), this, SLOT(OnEnd()));
            }
        }
        else if (Item_Video == itr.key())
        {
            for (auto& obj : itr.value())
            {
                CVideoWidget* pVideo = new CVideoWidget;
                pVideo->setView(this);
                pVideo->resize(obj.m_realWidth, obj.m_realHeight);
                m_pScene->blockSignals(true);
                pVideo->setChannel(obj.m_iChannel);
                pVideo->setTitle(obj.m_strTitle);
                m_pScene->blockSignals(false);
                m_pScene->addWidget(pVideo);
                pVideo->move(-obj.m_realX, -obj.m_realY);

                // 绑定session与listener 
                QMap<int, CVideoFileSession*>::iterator iter = m_mapVideoSession.find(obj.m_iChannel); 
                if (iter != m_mapVideoSession.end() && iter.key() == obj.m_iChannel) // 存在 
                {
                    iter.value()->GetStreamMgr()->AddStream(pVideo);
                }
                else // 不存在 
                {
                    CVideoFileSession* pSession = new CVideoFileSession;
                    pSession->Init(m_strVideoStorage.toStdString().c_str(), obj.m_iChannel);
                    pSession->GetStreamMgr()->AddStream(pVideo);
                    m_mapVideoSession.insert(obj.m_iChannel, pSession);
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
                pItem->setXAxisRange(m_dtBegin.toMSecsSinceEpoch(), m_dtEnd.toMSecsSinceEpoch());
                m_pScene->blockSignals(true);
                pItem->setChannel(obj.m_iChannel);
                pItem->setTitle(obj.m_strTitle);
                pItem->setLines(obj.m_lstLines);
                m_pScene->blockSignals(false);

                // 绑定session与listener 
                QMap<int, CCanFileSession*>::iterator iter = m_mapCanSession.find(obj.m_iChannel);
                if (iter != m_mapCanSession.end() && iter.key() == obj.m_iChannel)       // 存在，直接绑定 
                {
                    iter.value()->GetStreamMgr()->AddStream(pItem);
                }
                else    // 不存在 
                {
                    CCanFileSession* pSession = new CCanFileSession;
                    pSession->Init(m_strCanStorage.toStdString().c_str(), &m_profile, obj.m_iChannel);
                    pSession->GetStreamMgr()->AddStream(pItem);
                    m_mapCanSession.insert(obj.m_iChannel, pSession);
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
                m_pScene->blockSignals(true);
                pTable->setChannel(obj.m_iChannel);
                pTable->setTitle(obj.m_strTitle);
                m_pScene->blockSignals(false);

                // 绑定session与listener 
                QMap<int, CCanFileSession*>::iterator iter = m_mapCanSession.find(obj.m_iChannel);
                if (iter != m_mapCanSession.end() && iter.key() == obj.m_iChannel)
                {
                    iter.value()->GetStreamMgr()->AddStream(pTable);
                }
                else    // 不存在 
                {
                    CCanFileSession* pSession = new CCanFileSession;
                    pSession->Init(m_strCanStorage.toStdString().c_str(), &m_profile, obj.m_iChannel);
                    pSession->GetStreamMgr()->AddStream(pTable);
                    m_mapCanSession.insert(obj.m_iChannel, pSession);
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
                tmpWgtPro.m_iChannel = dynamic_cast<CVideoWidget*>(pCustomItem)->getChannel();
                tmpWgtPro.m_realX = item->mapRectFromScene(item->boundingRect()).topLeft().toPoint().x();
                tmpWgtPro.m_realY = item->mapRectFromScene(item->boundingRect()).topLeft().toPoint().y();
                tmpWgtPro.m_realWidth = item->boundingRect().bottomRight().x() - item->boundingRect().topLeft().x();
                tmpWgtPro.m_realHeight = item->boundingRect().bottomRight().y() - item->boundingRect().topLeft().y();
                tmpWgtPro.m_strTitle = dynamic_cast<CVideoWidget*>(pCustomItem)->getTitle();
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
                tmpWgtPro.m_iChannel = dynamic_cast<CTableView*>(pCustomItem)->getChannel();
                tmpWgtPro.m_realX = item->mapRectFromScene(item->boundingRect()).topLeft().toPoint().x();
                tmpWgtPro.m_realY = item->mapRectFromScene(item->boundingRect()).topLeft().toPoint().y();
                tmpWgtPro.m_realWidth = item->boundingRect().bottomRight().x() - item->boundingRect().topLeft().x();
                tmpWgtPro.m_realHeight = item->boundingRect().bottomRight().y() - item->boundingRect().topLeft().y();
                tmpWgtPro.m_strTitle = dynamic_cast<CTableView*>(pCustomItem)->getTitle();

                //mapTmpItems[Item_Table].append(tmpWgtPro);
            }
        }
        else
        {
            tmpWgtPro.m_type = Item_Chart;
            tmpWgtPro.m_iChannel = dynamic_cast<CCurveGraphicsItem*>(item)->getChannel();
            tmpWgtPro.m_realX = item->mapRectFromScene(item->boundingRect()).topLeft().toPoint().x();
            tmpWgtPro.m_realY = item->mapRectFromScene(item->boundingRect()).topLeft().toPoint().y();
            tmpWgtPro.m_realWidth = item->boundingRect().bottomRight().x() - item->boundingRect().topLeft().x();
            tmpWgtPro.m_realHeight = item->boundingRect().bottomRight().y() - item->boundingRect().topLeft().y();
            tmpWgtPro.m_strTitle = dynamic_cast<CCurveGraphicsItem*>(item)->getTitle();
            tmpWgtPro.m_lstLines = dynamic_cast<CCurveGraphicsItem*>(item)->getLines();
            //mapTmpItems[Item_Chart].append(tmpWgtPro);
        }
        mapTmpItems[tmpWgtPro.m_type].append(tmpWgtPro);
    }

    CFileOperationManager cfm("das.xml");
    if (cfm.writeXmlFile(mapTmpItems, m_strStoragePath, m_dtBegin.toString("yyyy/MM/dd hh:mm:ss:zzz"), m_dtEnd.toString("yyyy/MM/dd hh:mm:ss:zzz")))
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
                // 时间轴只有一个 
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
                            QMessageBox::warning(this, trFormString(cstDictDas), trFormString("Time axis already exists!"), QMessageBox::Ok);
                            return;
                        }
                    }
                }

                CTimeAxis* pTimeAxis = new CTimeAxis;
                pTimeAxis->setEditModeEnabled(m_bEditFlag);
                m_pScene->addWidget(pTimeAxis);
                pTimeAxis->move(mapToScene(event->pos()).toPoint());
                pTimeAxis->setTimeRange(m_dtBegin.toString("yyyy/MM/dd hh:mm:ss:zzz"), m_dtEnd.toString("yyyy/MM/dd hh:mm:ss:zzz"));

                connect(pTimeAxis, SIGNAL(sigSkipTo(const QDateTime&)), this, SLOT(OnSkipTo(const QDateTime&)));
                connect(pTimeAxis, SIGNAL(sigEnd()), this, SLOT(OnEnd()));
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

// channel changed
void CGraphicsView::OnChannelChanged(QGraphicsItem* item, const int& iPreChannel)
{
    if (!item)
    {
        return;
    }

    if (item->isWidget())
    {
        QGraphicsProxyWidget* pWidget = (QGraphicsProxyWidget*)item;
        CCustomWidgetBase* pCustomItem = (CCustomWidgetBase*)pWidget->widget();
        ITEMTYPE iType = pCustomItem->type();
        if (iType == Item_Video)         // video 
        {
            CVideoWidget *pVideoWidget = dynamic_cast<CVideoWidget*>(pCustomItem);
            if (m_mapVideoSession.keys().contains(iPreChannel))
            {
                m_mapVideoSession.value(iPreChannel)->GetStreamMgr()->RemoveStream(pVideoWidget);
            }

            if (m_mapVideoSession.keys().contains(pVideoWidget->getChannel()))
            {
                m_mapVideoSession.value(pVideoWidget->getChannel())->GetStreamMgr()->AddStream(pVideoWidget);
            }
            else
            {
                CVideoFileSession* pVideoSession = new CVideoFileSession;
                pVideoSession->Init(m_strVideoStorage.toStdString().c_str(), pVideoWidget->getChannel());
                pVideoSession->GetStreamMgr()->AddStream(pVideoWidget);
                m_mapVideoSession.insert(pVideoWidget->getChannel(), pVideoSession);
            }
        }
        else if (iType == Item_Table)           // table 
        {
            CTableView *pTableView = dynamic_cast<CTableView*>(pCustomItem);
            if (m_mapCanSession.keys().contains(iPreChannel))
            {
                m_mapCanSession.value(iPreChannel)->GetStreamMgr()->RemoveStream(pTableView);
            }

            if (m_mapCanSession.keys().contains(pTableView->getChannel()))
            {
                m_mapCanSession.value(pTableView->getChannel())->GetStreamMgr()->AddStream(pTableView);
            }
            else
            {
                CCanFileSession* pCanSession = new CCanFileSession;
                pCanSession->Init(m_strCanStorage.toStdString().c_str(), &m_profile, pTableView->getChannel());
                pCanSession->GetStreamMgr()->AddStream(pTableView);
                m_mapCanSession.insert(pTableView->getChannel(), pCanSession);
            }
        }
    }
    else
    {
        CCurveGraphicsItem* pItem = dynamic_cast<CCurveGraphicsItem*>(item);
        if (m_mapCanSession.keys().contains(iPreChannel))
        {
            m_mapCanSession.value(iPreChannel)->GetStreamMgr()->RemoveStream(pItem);
        }

        if (m_mapCanSession.keys().contains(pItem->getChannel()))
        {
            m_mapCanSession.value(pItem->getChannel())->GetStreamMgr()->AddStream(pItem);
        }
        else
        {
            CCanFileSession* pCanSession = new CCanFileSession;
            pCanSession->Init(m_strCanStorage.toStdString().c_str(), &m_profile, pItem->getChannel());
            pCanSession->GetStreamMgr()->AddStream(pItem);
            m_mapCanSession.insert(pItem->getChannel(), pCanSession);
        }
    }

    // save
    saveLayout();
}


void CGraphicsView::OnSkipTo(const QDateTime& currentDt)
{
    skipTo(currentDt);
}


void CGraphicsView::OnEnd()
{
    m_pScene->setPlayFlag(false);
    emit sigEnd();
}

