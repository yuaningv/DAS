/*
* File    : CVideoWidget.h
*
* Date    : 2017-04-25
*
* Author  : 
*
* Brief   : 
*
* Note    :
*/

#ifndef CVideoWidget_h
#define CVideoWidget_h


#include "CCustomWidgetBase.h"

class QLabel;

typedef struct VideoItem
{
    int iID;                // channel 
    int iPosX;
    int iPoxY;
    int iWidth;
    int iHeight;

} VideoItem_t;


class CVideoWidget : public CCustomWidgetBase
{
    Q_OBJECT

public:
    explicit CVideoWidget(QWidget* parent = 0);
    ~CVideoWidget();

public:
    void setTitle(const QString& strTitle);

private:
    QLabel* m_labelTitle;

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *ev);
    virtual void keyReleaseEvent(QKeyEvent *ev);

private:

};


#endif // CVideoWidget_h

