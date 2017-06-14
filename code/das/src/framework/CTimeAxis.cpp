#include "CTimeAxis.h"
#include "CLogManager.h"

#include "QtWidgets/QLabel"
#include "QtWidgets/QSlider"
#include "QtWidgets/QHBoxLayout"
#include "QtCore/QDateTime"


CTimeAxis::CTimeAxis(QWidget* parent /*= 0*/)
    : CCustomWidgetBase(parent)
    , m_pLbEndTime(nullptr)
    , m_pSlider(nullptr),
    m_bProgressSlider(false),
    m_iInterval(50)
{
    initLayout();

    m_pTimer = new QTimer(this);
    m_pTimer->setTimerType(Qt::PreciseTimer);
    m_pTimer->setInterval(m_iInterval);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(OnUpdate()));
}


CTimeAxis::~CTimeAxis()
{

}


// 初始化布局
void CTimeAxis::initLayout() 
{
    m_pSlider = new QSlider(Qt::Horizontal, this);
    m_pSlider->setTickPosition(QSlider::TicksBothSides);
    m_pSlider->setTickInterval(10000000);
    m_pSlider->setRange(0, 100000000);
    m_pSlider->setToolTip(QString::number(m_pSlider->value()));

    // 默认开始时间和结束时间 
    QDateTime dt = QDateTime::currentDateTime();
    m_dtStartTime = dt.addSecs(-600);
    m_dtEndTime = dt;
    m_dtCurrentTime = m_dtStartTime;

    // 结束时间 
    m_pLbEndTime = new QLabel(this);
    m_pLbEndTime->setText(dt.toString("yyyy/MM/dd\nhh:mm:ss:zzz"));

    // 开始时间 
    m_pLbScale1 = new QLabel(this);
    m_pLbScale1->setText(dt.addSecs(-600).toString("yyyy/MM/dd\nhh:mm:ss:zzz"));

    m_pLbScale2 = new QLabel(this);
    m_pLbScale2->setText(dt.addSecs(-540).toString("hh:mm:ss:zzz"));
    m_pLbScale3 = new QLabel(this);
    m_pLbScale3->setText(dt.addSecs(-480).toString("yyyy/MM/dd\nhh:mm:ss:zzz"));
    m_pLbScale4 = new QLabel(this);
    m_pLbScale4->setText(dt.addSecs(-420).toString("hh:mm:ss:zzz"));
    m_pLbScale5 = new QLabel(this);
    m_pLbScale5->setText(dt.addSecs(-360).toString("yyyy/MM/dd\nhh:mm:ss:zzz"));
    m_pLbScale6 = new QLabel(this);
    m_pLbScale6->setText(dt.addSecs(-300).toString("hh:mm:ss:zzz"));
    m_pLbScale7 = new QLabel(this);
    m_pLbScale7->setText(dt.addSecs(-240).toString("yyyy/MM/dd\nhh:mm:ss:zzz"));
    m_pLbScale8 = new QLabel(this);
    m_pLbScale8->setText(dt.addSecs(-180).toString("hh:mm:ss:zzz"));
    m_pLbScale9 = new QLabel(this);
    m_pLbScale9->setText(dt.addSecs(-120).toString("yyyy/MM/dd\nhh:mm:ss:zzz"));
    m_pLbScale10 = new QLabel(this);
    m_pLbScale10->setText(dt.addSecs(-60).toString("hh:mm:ss:zzz"));

    QHBoxLayout* pLayoutLabel = new QHBoxLayout(this);
    pLayoutLabel->setSpacing(0);
    pLayoutLabel->setContentsMargins(0, 0, 0, 0);
    pLayoutLabel->addWidget(m_pLbScale1);
    pLayoutLabel->addWidget(m_pLbScale2);
    pLayoutLabel->addWidget(m_pLbScale3);
    pLayoutLabel->addWidget(m_pLbScale4);
    pLayoutLabel->addWidget(m_pLbScale5);
    pLayoutLabel->addWidget(m_pLbScale6);
    pLayoutLabel->addWidget(m_pLbScale7);
    pLayoutLabel->addWidget(m_pLbScale8);
    pLayoutLabel->addWidget(m_pLbScale9);
    pLayoutLabel->addWidget(m_pLbScale10);
    QWidget* pWidget = new QWidget;
    pWidget->setLayout(pLayoutLabel);

    QVBoxLayout* pLayoutV = new QVBoxLayout(this);
    pLayoutV->addWidget(m_pSlider);
    pLayoutV->addWidget(pWidget);

    QWidget* pWidget2 = new QWidget;
    pWidget2->setLayout(pLayoutV);

    QHBoxLayout* pTmpLayout = new QHBoxLayout(this);
    pTmpLayout->addWidget(pWidget2);
    pTmpLayout->addWidget(m_pLbEndTime);

    this->setLayout(pTmpLayout);

    connect(m_pSlider, SIGNAL(valueChanged(int)), this, SLOT(OnProgressChanged(int)));
}



ITEMTYPE CTimeAxis::type()
{
    return Item_TimeAxis;
}


void CTimeAxis::setTimeRange(const QString& strStart, const QString& strEnd)
{
    m_dtStartTime = QDateTime::fromString(strStart, "yyyy/MM/dd hh:mm:ss:zzz");
    m_dtEndTime = QDateTime::fromString(strEnd, "yyyy/MM/dd hh:mm:ss:zzz");
    m_dtCurrentTime = m_dtStartTime;

    // 开始时间 
    QString strTempStart = strStart;
    strTempStart.replace(" ", "\n");
    m_pLbScale1->setText(strTempStart);

    // 刻度 
    int iOffset = m_dtStartTime.secsTo(m_dtEndTime) / 10;

    m_pLbScale2->setText(m_dtStartTime.addSecs(iOffset).toString("hh:mm:ss:zzz"));
    m_pLbScale3->setText(m_dtStartTime.addSecs(2 * iOffset).toString("yyyy/MM/dd\nhh:mm:ss:zzz"));
    m_pLbScale4->setText(m_dtStartTime.addSecs(3 * iOffset).toString("hh:mm:ss:zzz"));
    m_pLbScale5->setText(m_dtStartTime.addSecs(4 * iOffset).toString("yyyy/MM/dd\nhh:mm:ss:zzz"));
    m_pLbScale6->setText(m_dtStartTime.addSecs(5 * iOffset).toString("hh:mm:ss:zzz"));
    m_pLbScale7->setText(m_dtStartTime.addSecs(6 * iOffset).toString("yyyy/MM/dd\nhh:mm:ss:zzz"));
    m_pLbScale8->setText(m_dtStartTime.addSecs(7 * iOffset).toString("hh:mm:ss:zzz"));
    m_pLbScale9->setText(m_dtStartTime.addSecs(8 * iOffset).toString("yyyy/MM/dd\nhh:mm:ss:zzz"));
    m_pLbScale10->setText(m_dtStartTime.addSecs(9 * iOffset).toString("hh:mm:ss:zzz"));

    // 结束时间 
    QString strTempEnd = strEnd;
    strTempEnd.replace(" ", "\n");
    m_pLbEndTime->setText(strTempEnd);
}


void CTimeAxis::setValue(const QString& strValue)
{
    // 当前时间 
    m_dtCurrentTime = QDateTime::fromString(strValue, "yyyy/MM/dd hh:mm:ss:zzz");
    if (m_dtCurrentTime >= m_dtEndTime)
    {
        m_bProgressSlider = true;
        m_pSlider->setValue(m_pSlider->minimum());
        m_bProgressSlider = false;
        m_dtCurrentTime = m_dtStartTime;
        m_pTimer->stop();
        emit sigEnd();
        return;
    }

    qint64 iCurrentTime = m_dtCurrentTime.toMSecsSinceEpoch();
    qint64 iMaxTime = m_dtEndTime.toMSecsSinceEpoch();
    qint64 iMinTime = m_dtStartTime.toMSecsSinceEpoch();
    qint64 iTimeOffset = iMaxTime - iMinTime;
    uint iOffset = m_pSlider->maximum() - m_pSlider->minimum();

    if (iTimeOffset <= 0)
    {
        return;
    }
    int iProgressValue = m_pSlider->minimum() + (iCurrentTime - iMinTime) * (1.0f * iOffset / iTimeOffset);
    m_bProgressSlider = true;
    m_pSlider->setValue(iProgressValue);
    m_bProgressSlider = false;
    m_pSlider->setToolTip(m_dtCurrentTime.toString("yyyy/MM/dd hh:mm:ss:zzz"));
}


QString CTimeAxis::getValue() const
{
    return m_dtCurrentTime.toString("yyyy/MM/dd hh:mm:ss:zzz");
}


QString CTimeAxis::getStartTime() const 
{ 
    return m_dtStartTime.toString("yyyy/MM/dd hh:mm:ss:zzz");
}


QString CTimeAxis::getEndTime() const 
{
    return m_dtEndTime.toString("yyyy/MM/dd hh:mm:ss:zzz");
}


void CTimeAxis::play()
{
    m_dtBeginTime = m_dtToUpdateTime = QDateTime::currentDateTime();

    m_pTimer->start();
}


void CTimeAxis::pause()
{
    m_pTimer->stop();
}


void CTimeAxis::setStep(int iStep)
{
    if (iStep < 0)
    {
        m_iInterval = m_iInterval * iStep;
    }
    else
    {
        m_iInterval = m_iInterval / iStep;
    }

    m_pTimer->setInterval(m_iInterval);
}


void CTimeAxis::OnProgressChanged(int iValue)
{
    if (m_bProgressSlider)          // 当拖动滚动条时，才响应该槽 
    {
        m_bProgressSlider = false;
        return;
    }

    // 拖动滑条，更新时间进度 
    uint iOffset = m_pSlider->maximum() - m_pSlider->minimum();
    qint64 iMaxTime = m_dtEndTime.toMSecsSinceEpoch();
    qint64 iMinTime = m_dtStartTime.toMSecsSinceEpoch();
    qint64 iTimeOffset = iMaxTime - iMinTime;
    qint64 iCurrentTime = iMinTime + (iValue - m_pSlider->minimum()) * (1.0f * iTimeOffset / iOffset);
    m_dtCurrentTime = QDateTime::fromMSecsSinceEpoch(iCurrentTime);
    m_pSlider->setToolTip(m_dtCurrentTime.toString("yyyy/MM/dd hh:mm:ss:zzz"));

    emit sigSkipTo(m_dtCurrentTime);
}


void CTimeAxis::OnUpdate()
{
    m_dtToUpdateTime = QDateTime::currentDateTime();
    quint64 TmpMsecs = m_dtToUpdateTime.toMSecsSinceEpoch() - m_dtBeginTime.toMSecsSinceEpoch();
    m_dtBeginTime = m_dtToUpdateTime;
    // 当前时间 
    //m_dtCurrentTime = m_dtCurrentTime.addMSecs(60 * 60 * 1000);
    //m_dtCurrentTime = m_dtCurrentTime.addMSecs(m_iInterval);
    m_dtCurrentTime = m_dtCurrentTime.addMSecs(TmpMsecs);
    setValue(m_dtCurrentTime.toString("yyyy/MM/dd hh:mm:ss:zzz"));
    //CLogManager::getInstance()->log(eLogDebug, "CTimeAxis::OnUpdate", QString("system:%1, time:%2").arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss:zzz")).arg(m_dtCurrentTime.toString("yyyy/MM/dd hh:mm:ss:zzz")).toStdString().c_str());

}


