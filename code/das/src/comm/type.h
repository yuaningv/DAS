#ifndef TYPE_H
#define TYPE_H

#include "QtGui/QColor"
#include "QtCore/QVector"
#include "QtCore/QMetaType"

enum ITEMTYPE
{
    Item_None = 0,
    Item_TimeAxis = 1,
    Item_Video = 2,
    Item_Chart = 3,
    Item_Table = 4
};

// 拖拽方向
enum DragDirection{
    NONE = 0,
    UP = 1,
    DOWN = 2,
    LEFT = 3,
    RIGHT = 4,
    LEFTTOP = 5,
    LEFTBOTTOM = 6,
    RIGHTBOTTOM = 7,
    RIGHTTOP = 8
};

#define PADDING 5 // margin
#define CurveRole Qt::UserRole + 100


typedef struct CurveLine
{
    QColor m_color;
    QString m_strName;
    QString m_strDisplayName;
    qreal m_realMin;
    qreal m_realMax;
    QVector<QPointF> m_vecPoints;
    QString m_strValue;

    CurveLine()
    {
        m_color = QColor();
        m_strName.clear();
        m_strDisplayName.clear();
        m_realMin = 0.0;
        m_realMax = 1.0;
        m_vecPoints.clear();
        m_strValue.clear();
    }

    bool operator==(const CurveLine& tmpObj)
    {
        if (tmpObj.m_strName == this->m_strName
            && tmpObj.m_strDisplayName == this->m_strDisplayName
            && tmpObj.m_realMin == this->m_realMin
            && tmpObj.m_realMax == this->m_realMax)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}CurveLine_t;

Q_DECLARE_METATYPE(CurveLine_t);

// 控件属性
typedef struct WidgetProperty
{
    ITEMTYPE m_type;
    qreal m_realX;
    qreal m_realY;
    qreal m_realWidth;
    qreal m_realHeight;
    QString m_strStart;
    QString m_strEnd;
    QString m_strPlayPos;
    int m_iChannel;
    QString m_strTitle;
    QList<CurveLine_t> m_lstLines;

    WidgetProperty()
    {
        m_type = Item_None;
        m_realX = 0.0;
        m_realY = 0.0;
        m_realWidth = 0.0;
        m_realHeight = 0.0;
        m_strStart.clear();
        m_strEnd.clear();
        m_strPlayPos.clear();
        m_iChannel = -1;
        m_strTitle.clear();
        m_lstLines.clear();
    }

}WidgetProperty_t; 


typedef struct ItemAttribute
{
    int iID;
    ITEMTYPE iType;
    QString strTitle;
    int iChannel;
    int iPosX;
    int iPoxY;
    int iWidth;
    int iHeight;

} ItemAttribute_t;


typedef struct 
{
    unsigned int iWidth; 
    unsigned int iHeight; 
} imageInfo_t;


#endif // TYPE_H
