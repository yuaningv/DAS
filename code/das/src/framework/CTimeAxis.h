/*
* File    : CTimeAxis.h
*
* Date    : 2017-04-25
*
* Author  : 
*
* Brief   : 时间轴控件
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
	QString getStartTime() const;
	QString getEndTime() const;
	QString getSliderPosition() const;

private:
    void initLayout();

protected:
    virtual ITEMTYPE type();

private:
    QLabel* m_pLbStartTime;
    QLabel* m_pLbEndTime;
    QSlider* m_pSlider;

    // 10个刻度标签
    QLabel* m_pLbScale1;   
    QLabel* m_pLbScale2;
    QLabel* m_pLbScale3;
    QLabel* m_pLbScale4;
    QLabel* m_pLbScale5;
    QLabel* m_pLbScale6;
    QLabel* m_pLbScale7;
    QLabel* m_pLbScale8;
    QLabel* m_pLbScale9;
    QLabel* m_pLbScale10;

};

#endif // CTIME_AXIS_H
