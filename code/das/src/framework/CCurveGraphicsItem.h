/*
* File    : CCurveGraphicsItem.h
*
* Date    : 2017-04-26
*
* Author  :
*
* Brief   : ����ͼ�ؼ�
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
    qreal m_dbYAxisMin;       //Y��С�̶�
    qreal m_dbYAxisMax;      // Y���̶�
    qreal m_realY;              // Y ��ÿ���̶ȴ�С
    qreal m_realYLength;        // Y��ÿ���̶ȳ���
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

    void setChannel(int iChannel) { m_iChannel = iChannel; };       // ͨ��channel���жϸ�����Ƿ��Ѿ��������ݣ�iChannel < 0�޹������ݣ�iChannel >= 0�������� 
    int getChannel() { return m_iChannel; };

    void setMouseLeftButtonRelease(bool bEnable) { m_bLeftPress = !bEnable; }

    void dragCurve(QGraphicsSceneMouseEvent* event);

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
    QPointF m_startPos;        // ��ק��ʼ��
    QPointF m_endPos;          // ��ק������
    DragDirection m_dragDir;   // ��ק����

    QString m_strStartTime;
    QString m_strEndTime;

    qreal m_dbXAxisMin;       //X��С�̶�
    qreal m_dbXAxisMax;      // X���̶�
    qreal m_realX;             // X��ÿ���̶ȴ�С
    qreal m_realXLength;        // X��ÿ���̶ȳ���

    qreal m_dbYAxisMin;       //Y��С�̶�
    qreal m_dbYAxisMax;      // Y���̶�
    qreal m_realY;              // Y ��ÿ���̶ȴ�С
    qreal m_realYLength;        // Y��ÿ���̶ȳ���

    int m_iXTicksCount;   // X��̶ȱ����
    int m_iYTicksCount;   // Y��̶ȱ����

    int m_iOffset;       // ��������߿���ƫ����

    qreal m_realScale;   // ���ű���

    qreal m_realXMaxDefault;  // X�����ֵ��ʼֵ
    qreal m_realXMinDefault;  // X�����ֵ��ʼֵ

    bool m_bEditFlag;    // �Ƿ��Ǳ༭ģʽ
    ITEMTYPE m_iType;

    QString m_strTitle;  // ͼ�α���

    QList<CurveLine_t> m_lstLines;

    QLine m_line;

    QMap<QString, CAxisData_t> m_mapAxis;

    int m_iChannel;
    QMutex m_mutex;

    QMap<QString, QList<QPointF>> m_mapPoints;    // ת��֮ǰ������<displayname, ponits>

    bool m_bLeftPress = false;
    QPointF m_CurveStartPos;        // �϶�������ʼ��
    QPointF m_CurveEndPos;          // �϶����߽�����

signals:

};


#endif
