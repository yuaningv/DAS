/*
* File    : CCurveGraphicsItem.h
*
* Date    : 2017-04-26
*
* Author  :
*
* Brief   : 曲线图控件
*
* Note    :
*/


#ifndef CCURVE_GRAPHICSITEM_H
#define CCURVE_GRAPHICSITEM_H

#include "QtWidgets/QGraphicsItem"
#include "QtCore/QObject"
#include "QtCore/QMutex"
#include "type.h"
#include "CCustomWidgetBase.h"

#include "constant.h"
#include "StreamMgr.h"
#include "CanDecoder.h"


typedef struct CAxisData
{
    qreal m_dbYAxisMin;       //Y最小刻度
    qreal m_dbYAxisMax;      // Y最大刻度
    qreal m_realY;              // Y 轴每个刻度大小
    qreal m_realYLength;        // Y轴每个刻度长度
    CAxisData()
    {
        m_dbYAxisMin = 0;
        m_dbYAxisMax = 0;
        m_realY = 0;
        m_realYLength = 0;
    }
}CAxisData_t;


class CCurveGraphicsItem : public QObject, public QGraphicsItem, public CStreamListener
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    explicit CCurveGraphicsItem(QGraphicsItem * parent = 0);
    ~CCurveGraphicsItem();

    void setTimeRange(const QString& strStart, const QString& strEnd);
    void setXAxisTicksCount(const int& iCount) { m_iXTicksCount = iCount; }
    void setYAxisTicksCount(const int& iCount) { m_iYTicksCount = iCount; }
    void setXAxisRange(const qreal& dbMin, const qreal& dbMax);
    void setYAxisRange(const qreal& dbMin, const qreal& dbMax) { m_dbYAxisMin = dbMin; m_dbYAxisMax = dbMax; }

    void setEnableEditMode(bool enable);
    void setTitle(const QString& strTitle) { m_strTitle = strTitle; update(boundingRect()); };
    QString getTitle() const { return m_strTitle; }

    void setType(ITEMTYPE iType) { m_iType = iType; };
    void setLines(const QList<CurveLine_t>& lstTmpVec);
    QList<CurveLine_t> getLines() const { return m_lstLines; }

    ITEMTYPE getType() { return m_iType; };

    QRectF boundingRect() const;
    void resetItemSize(const QRectF &rect);

    void setChannel(int iChannel) { m_iChannel = iChannel; };       // 通过channel来判断该组件是否已经关联数据，iChannel < 0无关联数据，iChannel >= 0关联数据 
    int getChannel() { return m_iChannel; };

private:
    QPointF& mapToAxis(const QString& strKeyName, QPointF& point) const;
    qreal YFromAxis(const QString& strKeyName, qreal yreal) const;
    qreal XFromAxis(qreal xreal) const;

    void ConvertPointsToAxis();

protected:
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

    virtual void hoverLeaveEnvet(QGraphicsSceneHoverEvent *event);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    void wheelEvent(QGraphicsSceneWheelEvent * event);

    //virtual HWND GetWndHandle();
    virtual void OnMedia(unsigned char* buffer, unsigned long length, unsigned long payload,
		CCustomDateTime* pTime, void* pCustomData);

private:
    QRectF m_itemRectF;
    QPointF m_startPos;        // 拖拽起始点
    QPointF m_endPos;          // 拖拽结束点
    DragDirection m_dragDir;   // 拖拽方向

    QString m_strStartTime;
    QString m_strEndTime;

    qreal m_dbXAxisMin;       //X最小刻度
    qreal m_dbXAxisMax;      // X最大刻度
    qreal m_realX;             // X轴每个刻度大小
    qreal m_realXLength;        // X轴每个刻度长度

    qreal m_dbYAxisMin;       //Y最小刻度
    qreal m_dbYAxisMax;      // Y最大刻度
    qreal m_realY;              // Y 轴每个刻度大小
    qreal m_realYLength;        // Y轴每个刻度长度

    int m_iXTicksCount;   // X轴刻度标记数
    int m_iYTicksCount;   // Y轴刻度标记数

    int m_iOffset;       // 坐标轴与边框间的偏移量

    qreal m_realScale;   // 缩放比例

    qreal m_realXMaxDefault;  // X轴最大值初始值
    qreal m_realXMinDefault;  // X轴最大值初始值

    bool m_bEditFlag;    // 是否是编辑模式
    ITEMTYPE m_iType;

    QString m_strTitle;  // 图形标题

    QList<CurveLine_t> m_lstLines;

    QLine m_line;

    QMap<QString, CAxisData_t> m_mapAxis;

    int m_iChannel;
    QMutex m_mutex;

    QMap<QString, QList<QPointF>> m_mapPoints;    // 转换之前的数据<displayname, ponits>

signals:

};


#endif
