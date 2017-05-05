#include "CTimeAxis.h"

#include "QtWidgets/QLabel"
#include "QtWidgets/QSlider"
#include "QtWidgets/QHBoxLayout"
#include "QtCore/QDateTime"


CTimeAxis::CTimeAxis(QWidget* parent /*= 0*/)
    : CCustomWidgetBase(parent)
    , m_pLbStartTime(nullptr)
    , m_pLbEndTime(nullptr)
    , m_pSlider(nullptr)
{
    initLayout();
}

CTimeAxis::~CTimeAxis()
{

}



ITEMTYPE CTimeAxis::type()
{
    return Item_TimeAxis;
}



/*
* 设置开始结束时间
* strStart 开始时间 strEnd 结束时间  时间格式 yyyy/MM/dd hh:mm:ss
*/
void CTimeAxis::setTimeRange(const QString& strStart, const QString& strEnd)
{
    m_pLbScale1->setText(strStart);
    m_pLbEndTime->setText(strEnd);

    uint iTmpMin = QDateTime::fromString(strStart, "yyyy/MM/dd hh:mm:ss").toTime_t();
    uint iTmpMax = QDateTime::fromString(strEnd, "yyyy/MM/dd hh:mm:ss").toTime_t();

    //m_pSlider->setRange(iTmpMin, iTmpMax);
}


// 设置滑块位置
void CTimeAxis::setSliderPosition(const QString& strValue)
{
    uint iTmpValue = QDateTime::fromString(strValue, "yyyy/MM/dd hh:mm:ss").toTime_t();
    uint iOffset = m_pSlider->maximum() - m_pSlider->minimum();
    if (iOffset == 0)
    {
        return;
    }
    uint iMaxTime = QDateTime::fromString(m_pLbEndTime->text().replace("\n", " ").trimmed(), "yyyy/MM/dd hh:mm:ss").toTime_t();
    uint iMinTime = QDateTime::fromString(m_pLbScale1->text().replace("\n", " ").trimmed(), "yyyy/MM/dd hh:mm:ss").toTime_t();
    int iTmpTime = iMaxTime - iMinTime;
    int iProgressValue = (1.0*iOffset*(iTmpValue - iMinTime) / iTmpTime) + 0.5 + m_pSlider->minimum();   // 四舍五入
    m_pSlider->setValue(iProgressValue);
}


QString CTimeAxis::getStartTime() const 
{ 
	//return m_pLbStartTime->text().trimmed();
    return m_pLbScale1->text().replace("\n", " ").trimmed();
}


QString CTimeAxis::getEndTime() const 
{ 
    return m_pLbEndTime->text().replace("\n", " ").trimmed();
}


QString CTimeAxis::getSliderPosition() const 
{ 
	int iSliderValue = m_pSlider->value();
	uint iOffset = m_pSlider->maximum() - m_pSlider->minimum();
    uint iMaxTime = QDateTime::fromString(m_pLbEndTime->text().replace("\n", " ").trimmed(), "yyyy/MM/dd hh:mm:ss").toTime_t();
    uint iMinTime = QDateTime::fromString(m_pLbScale1->text().replace("\n", " ").trimmed(), "yyyy/MM/dd hh:mm:ss").toTime_t();
    int iTmpTime = iMaxTime - iMinTime;
    uint iTmpValue = (iSliderValue - m_pSlider->minimum())*iTmpTime / iOffset + iMinTime;
	//uint iTmpValue = iSliderValue * (iOffset / (uint)(m_pSlider->maximum())) + (uint)(m_pSlider->minimum());
	QString strValue = QDateTime::fromTime_t(iTmpValue).toString("yyyy/MM/dd hh:mm:ss");
	return strValue;
}


// 初始化布局
void CTimeAxis::initLayout()
{
    //m_pLbStartTime = new QLabel(this);
    m_pLbEndTime = new QLabel(this);
    m_pSlider = new QSlider(Qt::Horizontal, this);
    m_pSlider->setTickPosition(QSlider::TicksBothSides);
    m_pSlider->setTickInterval(10);
    m_pSlider->setRange(1, 100);
    m_pSlider->setToolTip(QString::number(m_pSlider->value()));
    //m_pLbStartTime->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));
    m_pLbEndTime->setText(QDateTime::currentDateTime().addSecs(3600).toString("yyyy/MM/dd\nhh:mm:ss"));

    m_pLbScale1 = new QLabel(this);
    m_pLbScale1->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd\nhh:mm:ss"));
    m_pLbScale2 = new QLabel(this);
    m_pLbScale2->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd\nhh:mm:ss"));
    m_pLbScale3 = new QLabel(this);
    m_pLbScale3->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd\nhh:mm:ss"));
    m_pLbScale4 = new QLabel(this);
    m_pLbScale4->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd\nhh:mm:ss"));
    m_pLbScale5 = new QLabel(this);
    m_pLbScale5->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd\nhh:mm:ss"));

    m_pLbScale6 = new QLabel(this);
    m_pLbScale6->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd\nhh:mm:ss"));
    m_pLbScale7 = new QLabel(this);
    m_pLbScale7->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd\nhh:mm:ss"));
    m_pLbScale8 = new QLabel(this);
    m_pLbScale8->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd\nhh:mm:ss"));
    m_pLbScale9 = new QLabel(this);
    m_pLbScale9->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd\nhh:mm:ss"));
    m_pLbScale10 = new QLabel(this);
    m_pLbScale10->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd\nhh:mm:ss"));

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
    //pTmpLayout->addWidget(m_pLbStartTime);
    pTmpLayout->addWidget(pWidget2);
    pTmpLayout->addWidget(m_pLbEndTime);

    this->setLayout(pTmpLayout);
}
