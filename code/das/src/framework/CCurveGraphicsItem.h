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
#include "type.h"
#include "CCustomWidgetBase.h"


class CCurveGraphicsItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    explicit CCurveGraphicsItem(QGraphicsItem * parent = 0);
    ~CCurveGraphicsItem();

    void setTimeRange(const QString& strStart, const QString& strEnd);
    void setXAxisTicksCount(const int& iCount) { m_iXTicksCount = iCount; }
    void setYAxisTicksCount(const int& iCount) { m_iYTicksCount = iCount; }
    void setXAxisRange(const qreal& dbMin, const qreal& dbMax) { m_dbXAxisMin = dbMin; m_dbXAxisMax = dbMax; m_realXDefault = m_dbXAxisMax; }
    void setYAxisRange(const qreal& dbMin, const qreal& dbMax) { m_dbYAxisMin = dbMin; m_dbYAxisMax = dbMax; }

    void setEnableEditMode(bool enable);
    void setTitle(const QString& strTitle);

    void setType(ITEMTYPE iType) { m_iType = iType; };
    void setLines(const QList<CurveLine_t>& lstTmpVec) { m_lstLines = lstTmpVec; update(); }
    QList<CurveLine_t> getLines() const { return m_lstLines; }

    ITEMTYPE getType() { return m_iType; };


    QRectF boundingRect() const;
    void resetItemSize(const QRectF &rect);

private:
    QPointF& mapToAxis(QPointF& point) const;

protected:
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

    virtual void hoverLeaveEnvet(QGraphicsSceneHoverEvent *event);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    virtual void keyPressEvent(QKeyEvent * event);

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

    qreal m_realXDefault;  // X轴最大值初始值

    bool m_bEditFlag;    // 是否是编辑模式
    ITEMTYPE m_iType;

    QString m_strTitle;  // 图形标题

    QList<CurveLine_t> m_lstLines;

    QLine m_line;

signals:

};


#endif
