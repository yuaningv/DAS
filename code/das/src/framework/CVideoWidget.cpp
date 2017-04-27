#include "CVideoWidget.h"

#include "QtGui/QKeyEvent"
#include "QtWidgets/QLabel"


CVideoWidget::CVideoWidget(QWidget* parent /*= 0*/)
    : CCustomWidgetBase(parent)
{
    resize(500, 400);

    m_labelTitle = new QLabel(this);
    m_labelTitle->setGeometry(10, 0, this->width(), 25);
    m_labelTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_labelTitle->setText("Table");
}


CVideoWidget::~CVideoWidget()
{
}



void CVideoWidget::setTitle(const QString& strTitle)
{
    m_labelTitle->setText(strTitle);
}

void CVideoWidget::mouseDoubleClickEvent(QMouseEvent *ev)
{
}


void CVideoWidget::keyReleaseEvent(QKeyEvent *ev)
{
}


