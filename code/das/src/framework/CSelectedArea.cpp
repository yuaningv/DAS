#include "CSelectedArea.h"
#include "QtWidgets/QApplication"
#include "QtWidgets/QDesktopWidget"
#include "QtGui/QPainter"
#include "QtGui/QPen"
#include "QtGui/QMouseEvent"
#include "constant.h"
//#include "aps_global.h"

const char* dictEnterScreenshots = "Space/Enter key enter screenshots";						//Space/Enter键 确认截屏
const char* dictCancelScreenshots = "ESCAPE/Alt+F4 key cancel screenshots";					//ESCAPE/Alt+F4键 取消截屏

CSelectedArea::CSelectedArea(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowOpacity(0.5);
    setWindowModality(Qt::WindowModal);
    setGeometry(QApplication::desktop()->geometry());

    m_bMousePressed = false;
    m_iStartX = -1;
    m_iStartY = -1;
    m_iEndX = -1;
    m_iEndY = -1;
}

CSelectedArea::~CSelectedArea()
{
}


void CSelectedArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // 绘制全屏背景色
    painter.setBrush(Qt::black);
    painter.setPen(Qt::black);
    painter.drawRect(rect());

    // 选中矩形框颜色
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::white);
    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);
    painter.setBrush(Qt::white);
    painter.drawRect(m_iStartX, m_iStartY, m_iEndX-m_iStartX, m_iEndY-m_iStartY);

    // 提示
    QString textHint;
    textHint = trFormString(dictEnterScreenshots);
    textHint+= "\n";
    textHint+= trFormString(dictCancelScreenshots);

    QFont font;
    font.setPixelSize(30);
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(rect(), Qt::AlignCenter, textHint);
}


void CSelectedArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bMousePressed = true;
        m_iStartX = event->x();
        m_iStartY = event->y();

        setCursor(QCursor(Qt::CrossCursor));
    }
    else if(event->button() == Qt::RightButton)
    {
        close();
    }
}


void CSelectedArea::mouseReleaseEvent(QMouseEvent *event)
{
    m_bMousePressed = false;
    setCursor(QCursor(Qt::ArrowCursor));

    if(event->button() == Qt::MiddleButton)
    {
        close();
        emit sigSelectedArea(QRect(m_iStartX, m_iStartY, m_iEndX-m_iStartX, m_iEndY-m_iStartY).normalized());
    }
}


void CSelectedArea::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bMousePressed)
    {
        m_iEndX = event->x();
        m_iEndY = event->y();
        update();
    }
}

void CSelectedArea::mouseDoubleClickEvent(QMouseEvent *event)
{
    //close();
    //emit sigSelectedArea(QRect(m_iStartX, m_iStartY, m_iEndX-m_iStartX, m_iEndY-m_iStartY).normalized());
}

void CSelectedArea::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
}


void CSelectedArea::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        close();
    }
    else if(event->key() == Qt::Key_Space || event->key() == Qt::Key_Return)
    {
        if(m_iStartX > m_iEndX)
        {
            int iTemp = m_iStartX;
            m_iStartX = m_iEndX;
            m_iEndX = iTemp;
        }

        if(m_iStartY > m_iEndY)
        {
            int iTemp = m_iStartY;
            m_iStartY = m_iEndY;
            m_iEndY = iTemp;
        }

        close();
        emit sigSelectedArea(QRect(m_iStartX, m_iStartY, m_iEndX-m_iStartX, m_iEndY-m_iStartY).normalized());
    }
}

