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
#include "QtCore/QDateTime"

class QLabel;
class QSlider;


class CTimeAxis : public CCustomWidgetBase
{
    Q_OBJECT

public:
    explicit CTimeAxis(QWidget* parent = 0);
    ~CTimeAxis();

public:
    void setTimeRange(const QString& strStart, const QString& strEnd);

    void setValue(const QString& strValue);
    QString getValue() const;

    QString getStartTime() const;
    QString getEndTime() const;

private:
    void initLayout();

protected:
    virtual ITEMTYPE type();

private:
    QSlider* m_pSlider;

    QLabel* m_pLbEndTime;

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

    QDateTime m_dtStartTime;
    QDateTime m_dtEndTime;
    QDateTime m_dtCurrentTime;      // 当前时间 

    bool m_bProgressSlider;

signals:

private slots:
    void OnProgressChanged(int iValue);
};

#endif // CTIME_AXIS_H 

