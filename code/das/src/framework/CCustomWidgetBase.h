/*
* File    : CCustomWidgetBase.h
*
* Date    : 2017-04-25
*
* Author  : 
*
* Brief   : 自定义控件基类
*
* Note    :
*/


#ifndef CCUSTOM_WIDGET_BASE_H
#define CCUSTOM_WIDGET_BASE_H


#include "QtWidgets/QFrame"
#include "type.h"


class CCustomWidgetBase : public QFrame
{
    Q_OBJECT

public:
    CCustomWidgetBase(QWidget* parent = 0);
    virtual ~CCustomWidgetBase() = 0;

public:
    virtual ITEMTYPE type() = 0;
    void setEditModeEnabled(bool enable) { m_bEditFlag = enable; }

private:
    void mouseRegion(const QPoint &cursorPoint);

protected:
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);

protected:
    bool m_bIsLeftPressDown;       // 是否左键按下
    QPoint m_startPos;             // 拖动开始点
    QPoint m_endPos;               // 拖动结束点
    DragDirection m_dragDirection; // 拖拽方向
    bool m_bEditFlag;               // 是否是编辑模式
};


#endif // CCUSTOM_WIDGET_BASE_H
