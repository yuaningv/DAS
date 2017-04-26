/*
* File    : CTimeAxis.h
*
* Date    : 2017-04-25
*
* Author  : 
*
* Brief   : Ê±¼äÖá¿Ø¼þ
*
* Note    :
*/


#ifndef CTIME_AXIS_H
#define CTIME_AXIS_H


#include "CCustomWidgetBase.h"

class QLabel;
class QSlider;

class CTimeAxis : public CCustomWidgetBase
{
    Q_OBJECT

public:
    explicit CTimeAxis(QWidget* parent = 0);
    ~CTimeAxis();

    void setTimeRange(const QString& strStart, const QString& strEnd);
    void setSliderPosition(const QString& strValue);

private:
    void initLayout();

protected:
 

private:
    QLabel* m_pLbStartTime;
    QLabel* m_pLbEndTime;
    QSlider* m_pSlider;

};

#endif // CTIME_AXIS_H
