#include "CVideoWidget.h"

#include "QtGui/QKeyEvent"
#include "QtWidgets/QLabel"


CVideoWidget::CVideoWidget(QWidget* parent /*= 0*/)
    : CCustomWidgetBase(parent),
    m_bFullscreenFlag(false),
    m_bFlag(true)
{
    static int iVideoSquenceNumber = 1;
    m_iID = iVideoSquenceNumber++;
    m_strTitle = QString("Video%1").arg(m_iID);

    resize(500, 400);

    m_labelTitle = new QLabel(this);
    m_labelTitle->setGeometry(10, 0, this->width(), 25);
    m_labelTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_labelTitle->setText(m_strTitle);

    m_iLastX = this->x();
    m_iLastY = this->y();
    m_iLastWidth = this->width();
    m_iLastHeight = this->height();
}


CVideoWidget::~CVideoWidget()
{
}



void CVideoWidget::setTitle(const QString& strTitle)
{
    m_labelTitle->setText(strTitle);
    m_strTitle = strTitle;
}


void CVideoWidget::updateFrame()
{
    if (m_bFlag)
    {
        QPalette pal;
        pal.setBrush(QPalette::Background, Qt::red);
        this->setPalette(pal);
        m_bFlag = false;
    }
    else
    {
        QPalette pal;
        pal.setBrush(QPalette::Background, Qt::green);
        this->setPalette(pal);
        m_bFlag = true;
    }
}


ITEMTYPE CVideoWidget::type()
{
    return Item_Video;
}

void CVideoWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    /*m_iPosX = this->x();
    m_iPosY = this->y();

    m_iLastX = m_pView->mapFromScene(QPoint(this->x(), this->y())).x();
    m_iLastY = m_pView->mapFromScene(QPoint(this->x(), this->y())).y();
    m_iLastWidth = this->width();
    m_iLastHeight = this->height();*/

    CCustomWidgetBase::mouseReleaseEvent(ev);
}


void CVideoWidget::mouseDoubleClickEvent(QMouseEvent *ev)
{
    if (m_pView != NULL)
    {
        m_iLastX = m_pView->mapFromScene(QPoint(this->x(), this->y())).x();
        m_iLastY = m_pView->mapFromScene(QPoint(this->x(), this->y())).y();
        m_iLastWidth = this->width();
        m_iLastHeight = this->height();

        int iWidth = m_pView->width();
        int iHeight = m_pView->height();
        this->resize(iWidth, iHeight);
        this->move(m_pView->mapToScene(QPoint(0, 0)).toPoint());

        m_bFullscreenFlag = true;
    }
}


void CVideoWidget::keyReleaseEvent(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Escape)
    {
        if (m_bFullscreenFlag)
        {
            m_bFullscreenFlag = false;
            this->resize(m_iLastWidth, m_iLastHeight);
            this->move(m_pView->mapToScene(QPoint(m_iLastX, m_iLastY)).toPoint());
        }
    }
}


