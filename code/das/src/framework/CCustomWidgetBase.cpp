#include "CCustomWidgetBase.h"

#include "QtGui/QMouseEvent"

CCustomWidgetBase::CCustomWidgetBase(QWidget* parent /*= 0*/)
    : QFrame(parent)
    , m_bIsLeftPressDown(false)
    , m_startPos(0, 0)
    , m_endPos(0, 0)
    , m_dragDirection(NONE)
    , m_bEditFlag(false)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    this->setMouseTracking(true);
}

CCustomWidgetBase::~CCustomWidgetBase()
{

}

/*
* 判断鼠标所在区域
* cursorPoint 当前鼠标所在点
*/
void CCustomWidgetBase::mouseRegion(const QPoint &cursorPoint)
{
    QRect rect = this->rect();
    QPoint pointTmpTopLeft = mapToParent(rect.topLeft());
    QPoint pointTmpRightBottom = mapToParent(rect.bottomRight());
    int iTmpX = cursorPoint.x();
    int iTmpY = cursorPoint.y();

    if (pointTmpTopLeft.x() + PADDING >= iTmpX && pointTmpTopLeft.x() <= iTmpX && pointTmpTopLeft.y() + PADDING >= iTmpY && pointTmpTopLeft.y() <= iTmpY)
    {
        // 左上角
        m_dragDirection = LEFTTOP;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else if (iTmpX >= pointTmpRightBottom.x() - PADDING && iTmpX <= pointTmpRightBottom.x() && iTmpY >= pointTmpRightBottom.y() - PADDING && iTmpY <= pointTmpRightBottom.y())
    {
        // 右下角
        m_dragDirection = RIGHTBOTTOM;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else if (iTmpX <= pointTmpTopLeft.x() + PADDING && iTmpX >= pointTmpTopLeft.x() && iTmpY >= pointTmpRightBottom.y() - PADDING && iTmpY <= pointTmpRightBottom.y())
    {
        //左下角
        m_dragDirection = LEFTBOTTOM;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    }
    else if (iTmpX <= pointTmpRightBottom.x() && iTmpX >= pointTmpRightBottom.x() - PADDING && iTmpY >= pointTmpTopLeft.y() && iTmpY <= pointTmpTopLeft.y() + PADDING)
    {
        // 右上角
        m_dragDirection = RIGHTTOP;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    }
    else if (iTmpX <= pointTmpTopLeft.x() + PADDING && iTmpX >= pointTmpTopLeft.x())
    {
        // 左边
        m_dragDirection = LEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if (iTmpX <= pointTmpRightBottom.x() && iTmpX >= pointTmpRightBottom.x() - PADDING)
    {
        // 右边
        m_dragDirection = RIGHT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if (iTmpY >= pointTmpTopLeft.y() && iTmpY <= pointTmpTopLeft.y() + PADDING)
    {
        // 上边
        m_dragDirection = UP;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }
    else if (iTmpY <= pointTmpRightBottom.y() && iTmpY >= pointTmpRightBottom.y() - PADDING)
    {
        // 下边
        m_dragDirection = DOWN;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }
    else
    {
        // 默认
        m_dragDirection = NONE;
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}


void CCustomWidgetBase::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_bEditFlag)
    {
        m_bIsLeftPressDown = false;
        if (m_dragDirection != NONE) 
        {
            this->releaseMouse();
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }

    QFrame::mouseReleaseEvent(event);
}

void CCustomWidgetBase::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_bEditFlag)
    {
        this->setCursor(QCursor(Qt::ArrowCursor));
        return QFrame::mouseMoveEvent(event);
    }
    QPoint mousePoint = mapToParent(event->pos());
    QRect rect = this->rect();
    QPoint pointTmpTopLeft = mapToParent(rect.topLeft());
    QPoint pointTmpRightBottom = mapToParent(rect.bottomRight());

    if (!m_bIsLeftPressDown)
    {
        this->mouseRegion(mousePoint);
    }
    else
    {
        if (m_dragDirection != NONE)
        {
            QRect rectMove(pointTmpTopLeft, pointTmpRightBottom);

            switch (m_dragDirection)
            {
            //左
            case LEFT:
                if (pointTmpRightBottom.x() - mousePoint.x() <= this->minimumWidth())
                {
                    rectMove.setX(pointTmpTopLeft.x());
                }
                else
                {
                    rectMove.setX(mousePoint.x());
                }
                break;

            // 右
            case RIGHT:
                rectMove.setWidth(mousePoint.x() - pointTmpTopLeft.x());
                break;

            // 上
            case UP:
                if (pointTmpRightBottom.y() - mousePoint.y() <= this->minimumHeight())
                {
                    rectMove.setY(pointTmpTopLeft.y());
                }
                else
                {
                    rectMove.setY(mousePoint.y());
                }
                break;

            // 下
            case DOWN:
                rectMove.setHeight(mousePoint.y() - pointTmpTopLeft.y());
                break;

            // 左上
            case LEFTTOP:
                if (pointTmpRightBottom.x() - mousePoint.x() <= this->minimumWidth())
                {
                    rectMove.setX(pointTmpTopLeft.x());
                }
                else
                {
                    rectMove.setX(mousePoint.x());
                }

                if (pointTmpRightBottom.y() - mousePoint.y() <= this->minimumHeight())
                {
                    rectMove.setY(pointTmpTopLeft.y());
                }
                else
                {
                    rectMove.setY(mousePoint.y());
                }
                break;

            // 右上
            case RIGHTTOP:
                rectMove.setWidth(mousePoint.x() - pointTmpTopLeft.x());
                if (pointTmpRightBottom.y() - mousePoint.y() <= this->minimumHeight())
                {
                    rectMove.setY(pointTmpTopLeft.y());
                }
                else
                {
                    rectMove.setY(mousePoint.y());
                }
                break;

            // 左下
            case LEFTBOTTOM:
                rectMove.setX(mousePoint.x());
                rectMove.setHeight(mousePoint.y() - pointTmpTopLeft.y());
                break;

            // 右下
            case RIGHTBOTTOM:
                rectMove.setWidth(mousePoint.x() - pointTmpTopLeft.x());
                rectMove.setHeight(mousePoint.y() - pointTmpTopLeft.y());
                break;

            default:
                break;
            }

            this->setGeometry(rectMove);
        }
        else
        {
            m_endPos = mousePoint;
            move(pointTmpTopLeft + (m_endPos - m_startPos));
            m_startPos = m_endPos;
            event->accept();
        }
    }
    QFrame::mouseMoveEvent(event);
}


void CCustomWidgetBase::mousePressEvent(QMouseEvent *event)
{
    if (!m_bEditFlag)
    {
        return QFrame::mousePressEvent(event);
    }
    switch (event->button())
    {
    case Qt::LeftButton:
        m_bIsLeftPressDown = true;
        if (m_dragDirection != NONE)
        {
            this->mouseGrabber();
        }
        else
        {
            m_startPos = mapToParent(event->pos());
            m_endPos = mapToParent(event->pos());
        }
        break;

    default:
        QFrame::mousePressEvent(event);
        break;
    }
}
