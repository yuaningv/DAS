/*
* File    : CToolBar.h
*
* Date    : 2017-04-25
*
* Author  : 
*
* Brief   : 自定义可拖拽控件工具栏
*
* Note    :
*/


#ifndef CTOOLBAR_H
#define CTOOLBAR_H


#include <QtWidgets/QToolBar>

class QLabel;

class CToolBar : public QToolBar
{
    Q_OBJECT

public:
    explicit CToolBar(QWidget *parent = 0);
    ~CToolBar();

protected:
    void dragEnterEvent(QDragEnterEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    // void dropEvent(QDropEvent *event);

private:
    QPoint m_startPos;
    QLabel* m_pChildAction;
};

#endif // CTOOLBAR_H
