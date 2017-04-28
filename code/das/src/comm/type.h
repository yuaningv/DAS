#ifndef TYPE_H
#define TYPE_H

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

// 控件属性
typedef struct WIDGET_PROPERTY
{
    ITEMTYPE m_type;
    qreal m_realX;
    qreal m_realY;
    qreal m_realWidth;
    qreal m_realHeight;
    QString m_strStart;
    QString m_strEnd;
    QString m_strPlayPos;

    WIDGET_PROPERTY()
    {
        m_type = Item_None;
        m_realX = 0.0;
        m_realY = 0.0;
        m_realWidth = 0.0;
        m_realHeight = 0.0;
        m_strStart.clear();
        m_strEnd.clear();
        m_strPlayPos.clear();
    }

}WidgetProperty; 


#endif // TYPE_H
