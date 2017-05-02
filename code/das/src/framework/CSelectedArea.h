/*
* File    : CSelectedArea.h
*
* Date    : 2017-04-25
*
* Author  :
*
* Brief   : ½ØÆÁÑ¡ÔñÇøÓò¿Ø¼þ
*
* Note    :
*/

#ifndef CSELECTEDAREA_H
#define CSELECTEDAREA_H

#include "QtWidgets/QWidget"


class CSelectedArea: public QWidget
{
    Q_OBJECT

public:
    CSelectedArea(QWidget *parent = 0);
    ~CSelectedArea();

private:
    int m_iStartX;
    int m_iStartY;
    int m_iEndX;
    int m_iEndY;

    bool m_bMousePressed;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void closeEvent(QCloseEvent *);

signals:
    void sigSelectedArea(const QRect& rectArea);
};

#endif // CSELECTEDAREA_H
