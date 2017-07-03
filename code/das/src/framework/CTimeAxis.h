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

#include "constant.h"

#include "CCustomWidgetBase.h"
#include "QtCore/QDateTime"
#include "QtCore/QTimer"

#include "StreamMgr.h"

class QLabel;
class QSlider;


class CTimeAxis : public CCustomWidgetBase, public CStreamListener
{
    Q_OBJECT

public:
    explicit CTimeAxis(QWidget* parent = 0);
    ~CTimeAxis();

public:
    void setValue(const QString& strValue);
    QString getValue() const;

    void setTimeRange(const QString& strStart, const QString& strEnd);
    QString getStartTime() const;
    QString getEndTime() const;

    void play();
    void pause();
    void setStep(int iStep);

	virtual void OnMedia(unsigned char* buffer, unsigned long length, unsigned long payload,
						CCustomDateTime* pTime, void* pCustomData);

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

	QDateTime m_dtPlayTime;
	QDateTime m_dtFromTime;
    QDateTime m_dtCurrentTime;      // 当前时间 

	QMutex m_mutex;

    bool m_bProgressSlider;
    QTimer* m_pTimer;
    int m_iInterval;
	int m_iStep;
signals:
    void sigSkipTo(const QDateTime& currentDt);
    void sigEnd();
    void sigTimeChanged();

private slots:
    void OnProgressChanged(int iValue);
    void OnUpdate();
};

#endif // CTIME_AXIS_H 

