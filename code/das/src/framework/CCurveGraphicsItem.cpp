#include "CCurveGraphicsItem.h"

#include "QtGui/QPainter"
#include "QtWidgets/QStyleOptionGraphicsItem"
#include "QtWidgets/QGraphicsSceneHoverEvent"
#include "QtWidgets/QGraphicsSceneMouseEvent"
#include "QtWidgets/QGraphicsScene"
#include "QtGui/QKeyEvent"
#include "QtWidgets/QGraphicsSceneContextMenuEvent"
#include "QtCore/QDebug"
#include "QtWidgets/QMenu"

static int index = 1;
CCurveGraphicsItem::CCurveGraphicsItem(QGraphicsItem * parent /*= 0*/)
    : QGraphicsItem(parent)
    , m_startPos(0, 0)
    , m_endPos(0, 0)
    , m_dragDir(NONE)
    , m_strStartTime("")
    , m_strEndTime("")
    , m_dbXAxisMin(0.0)
    , m_dbXAxisMax(1.0)
    , m_dbYAxisMin(0.0)
    , m_dbYAxisMax(1.0)
    , m_iXTicksCount(5)
    , m_iYTicksCount(5)
    , m_iOffset(50)
    , m_realScale(1.0)
    , m_realXDefault(1.0)
    , m_bEditFlag(false)
    , m_iType(Item_Chart)
{
    setAcceptDrops(true);
    setAcceptHoverEvents(true);
    setFlags(ItemIsFocusable);
    //setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges | ItemIsFocusable);


    // 初始大小
    m_itemRectF = QRectF(2, 2, 400, 300);
    m_strTitle = QString(tr("chart %1")).arg(index++);
    m_lstLines.clear();

    CurveLine_t c1;
    c1.m_color = Qt::red;
    c1.m_strName = "aa";
    c1.m_realMin = 0.0;
    c1.m_realMax = 1.0;
    c1.m_vecPoints = { QPointF(0, 0), QPointF(0.2, 0.1), QPointF(0.2, 0.1), QPointF(0.3, 0.5) };
    m_lstLines.append(c1);

    c1.m_color = Qt::blue;
    c1.m_strName = "bb";
    c1.m_realMin = 0.0;
    c1.m_realMax = 5.0;
    c1.m_vecPoints = { QPointF(0, 1), QPointF(0.1, 3), QPointF(0.1, 3), QPointF(0.3, 1.5) };
    m_lstLines.append(c1);

    c1.m_color = Qt::green;
    c1.m_strName = "cc";
    c1.m_realMin = 1.0;
    c1.m_realMax = 10.0;
    c1.m_vecPoints = { QPointF(0, 3), QPointF(0.3, 5), QPointF(0.3, 5), QPointF(0.6, 8) };
    m_lstLines.append(c1);

    m_line = QLine(0, 0, 0, 0);
}


CCurveGraphicsItem::~CCurveGraphicsItem()
{

}


void CCurveGraphicsItem::setTimeRange(const QString& strStart, const QString& strEnd)
{
    m_strStartTime = strStart;
    m_strEndTime = strEnd;
}

void CCurveGraphicsItem::setEnableEditMode(bool enable)
{ 
    m_bEditFlag = enable; 
    if (m_bEditFlag)
    {
        setFlags(ItemIsSelectable | ItemIsMovable | ItemSendsGeometryChanges | ItemIsFocusable);
    }
    else
    {
        setFlags(ItemIsFocusable);
    }
}

QRectF CCurveGraphicsItem::boundingRect() const
{
    return QRectF(m_itemRectF.x() - 2, m_itemRectF.y() - 2, m_itemRectF.width() + 4, m_itemRectF.height() + 4);
}

// item大小跟随拖拽改变
void CCurveGraphicsItem::resetItemSize(const QRectF &rect)
{ 
    m_itemRectF = rect; 
    update(boundingRect());
}

// 点转换为相对坐标轴的点
QPointF& CCurveGraphicsItem::mapToAxis(QPointF& point) const
{
    // 坐标轴的（0,0）点
    qreal x = m_itemRectF.bottomLeft().x() + m_iOffset;
    qreal y = m_itemRectF.bottomLeft().y() - m_iOffset;

    if (m_realX != 0)
    {
        qreal px = point.x() / m_realX * m_realXLength;
        point.setX(px + x);
    }
    if (m_realY != 0)
    {
        qreal py = point.y() / m_realY * m_realYLength;
        point.setY(y - py);
    }

    return point;
}

// 绘制曲线图
void CCurveGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0*/)
{
    Q_UNUSED(widget);


    if ((option->state & QStyle::State_Selected) == QStyle::State_Selected && m_bEditFlag)
    {
        painter->setPen(QPen(Qt::black, 1, Qt::DashLine));
        painter->drawLine(m_itemRectF.topLeft(), m_itemRectF.topRight());
        painter->drawLine(m_itemRectF.topLeft(), m_itemRectF.bottomLeft());
        painter->drawLine(m_itemRectF.bottomRight(), m_itemRectF.bottomLeft());
        painter->drawLine(m_itemRectF.bottomRight(), m_itemRectF.topRight());

    }
    painter->drawRect(m_itemRectF);
    painter->fillRect(m_itemRectF, Qt::white);

    painter->drawLine(m_line);


    // title
    painter->drawText(m_itemRectF.topLeft() + QPointF(15, 15), m_strTitle);

    // x坐标轴
    painter->drawLine(m_itemRectF.bottomLeft() + QPointF(m_iOffset, -m_iOffset), m_itemRectF.bottomRight() + QPointF(-m_iOffset, -m_iOffset));
    // 每个刻度值
    m_realX = (m_dbXAxisMax - m_dbXAxisMin) / m_iXTicksCount;
    //m_realX = floor(m_realX*10 + 0.5) / 10;
    // 每个刻度长度
    m_realXLength = ((m_itemRectF.bottomRight().x() - m_iOffset) - (m_itemRectF.bottomLeft().x() + m_iOffset)) / m_iXTicksCount;

    for (int i = 0; i < m_iXTicksCount; ++i)
    {
        painter->setPen(QPen(Qt::lightGray, 1, Qt::DashLine));
        painter->drawLine(m_itemRectF.bottomLeft().x() + m_iOffset + m_realXLength*(i + 1), m_itemRectF.bottomLeft().y() - m_iOffset, m_itemRectF.bottomLeft().x() + m_iOffset + m_realXLength*(i + 1), m_itemRectF.topLeft().y() + m_iOffset /*- 5*/);
        painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
        painter->drawText(m_itemRectF.bottomLeft().x() + m_iOffset + m_realXLength*(i + 1) - 5, m_itemRectF.bottomLeft().y() - m_iOffset + 10, QString::number(m_realX*(i + 1) + m_dbXAxisMin));
    }
    painter->drawText(m_itemRectF.bottomLeft().x() + m_iOffset - 5, m_itemRectF.bottomLeft().y() - m_iOffset + 10, QString::number(m_dbXAxisMin));

    // y 坐标轴
    painter->drawLine(m_itemRectF.bottomLeft() + QPointF(m_iOffset, -m_iOffset), m_itemRectF.topLeft() + QPointF(m_iOffset, m_iOffset));
    int iTmpLineCount = m_lstLines.count();
    // 每个图例位置长度
    int iLength = ((m_itemRectF.bottomRight().x()) - m_iOffset*2 - (m_itemRectF.bottomLeft().x() + m_iOffset*2)) / iTmpLineCount;

    for (int n = 0; n < iTmpLineCount; ++n)
    {
        setYAxisRange(m_lstLines.at(n).m_realMin, m_lstLines.at(n).m_realMax);

        // 每个刻度值
        m_realY = (m_dbYAxisMax - m_dbYAxisMin) / m_iYTicksCount;
        // 每个刻度长度
        m_realYLength = ((m_itemRectF.bottomLeft().y() - m_iOffset) - (m_itemRectF.topLeft().y() + m_iOffset)) / m_iYTicksCount;
        for (int i = 0; i < m_iYTicksCount; ++i)
        {
            painter->setPen(QPen(Qt::lightGray, 1, Qt::DashLine));
            painter->drawLine(m_itemRectF.bottomLeft().x() + m_iOffset, m_itemRectF.bottomLeft().y() - m_iOffset - m_realYLength*(i + 1), m_itemRectF.bottomRight().x() - m_iOffset/* + 5*/, m_itemRectF.bottomLeft().y() - m_iOffset - m_realYLength*(i + 1));
            painter->setPen(QPen(m_lstLines.at(n).m_color, 1, Qt::SolidLine));
            painter->drawText(m_itemRectF.bottomLeft().x() + m_iOffset - 20, m_itemRectF.bottomLeft().y() - m_iOffset - m_realYLength*(i + 1)-n*10 + 10, QString::number(m_realY*(i + 1) + m_dbYAxisMin));
        }
        painter->drawText(m_itemRectF.bottomLeft().x() + m_iOffset - 20, m_itemRectF.bottomLeft().y() - m_iOffset-n*10 + 10, QString::number(m_dbYAxisMin));

        // 图例
        painter->fillRect(m_itemRectF.bottomLeft().x() + m_iOffset*2 + iLength*n, m_itemRectF.bottomLeft().y() - 20, 5, 5, m_lstLines.at(n).m_color);
        painter->drawText(m_itemRectF.bottomLeft().x() + m_iOffset*2 + iLength*n + 15, m_itemRectF.bottomLeft().y() - 15, m_lstLines.at(n).m_strName);

        // 绘制曲线图
        QVector<QPointF> vecTmpPoint;
        for (QPointF p : m_lstLines[n].m_vecPoints)
        {
            // 转换成坐标轴坐标
            QPointF tmpP = p;
            vecTmpPoint.append(mapToAxis(p));
            qDebug() << "p.x = " << p.x();
            qDebug() << "line.x = " << m_line.p1().x();

            if (p.x() >= m_line.p1().x() - 0.5 && p.x() <= m_line.p1().x() + 0.5)
            {
                painter->drawText(p + QPointF(5, 0), QString::number(tmpP.y()));
            }
        }
        painter->drawLines(vecTmpPoint);
    }
}


void CCurveGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (!isSelected() && m_bEditFlag)
    {
        setCursor(Qt::ArrowCursor);
        m_dragDir = NONE;
        return QGraphicsItem::hoverEnterEvent(event);
    }

    if (!m_bEditFlag)
    {
        setCursor(Qt::ArrowCursor);
        return QGraphicsItem::hoverEnterEvent(event);
    }

    QPointF pos1 = event->scenePos();
    QPointF LeftTopPointF = this->scenePos() + QPointF(m_itemRectF.x(), m_itemRectF.y());
    QPointF RightBottomPointF = this->scenePos() + QPointF(m_itemRectF.x() + m_itemRectF.width(), m_itemRectF.y() + m_itemRectF.height());


    if ((event->pos().x() < 6)
        && (event->pos().y() < 6))
    {
        setCursor(Qt::SizeFDiagCursor);
        m_dragDir = LEFTTOP;
    }
    else if ((event->pos().x() > m_itemRectF.width() + 4 - 6)
        && (event->pos().y() < 6))
    {
        setCursor(Qt::SizeBDiagCursor);
        m_dragDir = RIGHTTOP;
    }
    else if ((event->pos().x() > m_itemRectF.width() + 4 - 6)
        && (event->pos().y() > m_itemRectF.height() + 4 - 6))
    {
        setCursor(Qt::SizeFDiagCursor);
        m_dragDir = RIGHTBOTTOM;
    }
    else if ((event->pos().x() < 6)
        && (event->pos().y() > m_itemRectF.height() + 4 - 6))
    {
        setCursor(Qt::SizeBDiagCursor);
        m_dragDir = LEFTBOTTOM;
    }
    else if ((pos1.y() >= LeftTopPointF.y() - 2)
        && (pos1.y() <= LeftTopPointF.y() + 2))
    {
        setCursor(Qt::SizeVerCursor);
        m_dragDir = UP;
    }
    else if ((pos1.y() <= RightBottomPointF.y() + 2)
        && (pos1.y() >= RightBottomPointF.y() - 2))
    {
        setCursor(Qt::SizeVerCursor);
        m_dragDir = DOWN;
    }
    else if ((pos1.x() >= LeftTopPointF.x() - 2)
        && (pos1.x() <= LeftTopPointF.x() + 2))
    {
        setCursor(Qt::SizeHorCursor);
        m_dragDir = LEFT;
    }

    else if ((pos1.x() <= RightBottomPointF.x() + 2)
        && (pos1.x() >= RightBottomPointF.x() - 2))
    {
        setCursor(Qt::SizeHorCursor);
        m_dragDir = RIGHT;
    }
    else
    {
        setCursor(Qt::SizeAllCursor);
        m_dragDir = NONE;
    }

    QGraphicsItem::hoverEnterEvent(event);
}


void CCurveGraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (!isSelected() && m_bEditFlag) 
    {
        setCursor(Qt::ArrowCursor);
        m_dragDir = NONE;
        return QGraphicsItem::hoverEnterEvent(event);
    }

    if (!m_bEditFlag)
    {
        setCursor(Qt::ArrowCursor);
        if (event->pos().x() >= m_itemRectF.bottomLeft().x() + m_iOffset && event->pos().x() <= m_itemRectF.bottomRight().x() - m_iOffset
            && event->pos().y() >= m_itemRectF.topLeft().y() + m_iOffset && event->pos().y() <= m_itemRectF.bottomLeft().y() - m_iOffset)
        {
            m_line.setLine(event->pos().x(), m_itemRectF.topLeft().y() + m_iOffset, event->pos().x(), m_itemRectF.bottomLeft().y() - m_iOffset);
        }
        else
        {
            m_line.setLine(0, 0, 0, 0);
        }
        return QGraphicsItem::hoverEnterEvent(event);
    }

    m_line.setLine(0, 0, 0, 0);

    QPointF pos1 = event->scenePos();
    QPointF LeftTopPointF = this->scenePos() + QPointF(m_itemRectF.x(), m_itemRectF.y());
    QPointF RightBottomPointF = this->scenePos() + QPointF(m_itemRectF.x() + m_itemRectF.width(), m_itemRectF.y() + m_itemRectF.height());


    if ((event->pos().x() < 6)
        && (event->pos().y() < 6))
    {
        setCursor(Qt::SizeFDiagCursor);
        m_dragDir = LEFTTOP;
    }
    else if ((event->pos().x() > m_itemRectF.width() + 4 - 6)
        && (event->pos().y() < 6))
    {
        setCursor(Qt::SizeBDiagCursor);
        m_dragDir = RIGHTTOP;
    }
    else if ((event->pos().x() > m_itemRectF.width() + 4 - 6)
        && (event->pos().y() > m_itemRectF.height() + 4 - 6))
    {
        setCursor(Qt::SizeFDiagCursor);
        m_dragDir = RIGHTBOTTOM;
    }
    else if ((event->pos().x() < 6)
        && (event->pos().y() > m_itemRectF.height() + 4 - 6))
    {
        setCursor(Qt::SizeBDiagCursor);
        m_dragDir = LEFTBOTTOM;
    }
    else if ((pos1.y() >= LeftTopPointF.y() - 2)
        && (pos1.y() <= LeftTopPointF.y() + 2))
    {
        setCursor(Qt::SizeVerCursor);
        m_dragDir = UP;
    }
    else if ((pos1.y() <= RightBottomPointF.y() + 2)
        && (pos1.y() >= RightBottomPointF.y() - 2))
    {
        setCursor(Qt::SizeVerCursor);
        m_dragDir = DOWN;
    }
    else if ((pos1.x() >= LeftTopPointF.x() - 2)
        && (pos1.x() <= LeftTopPointF.x() + 2))
    {
        setCursor(Qt::SizeHorCursor);
        m_dragDir = LEFT;
    }

    else if ((pos1.x() <= RightBottomPointF.x() + 2)
        && (pos1.x() >= RightBottomPointF.x() - 2))
    {
        setCursor(Qt::SizeHorCursor);
        m_dragDir = RIGHTBOTTOM;
    }
    else
    {
        setCursor(Qt::SizeAllCursor);
        m_dragDir = NONE;
    }

    QGraphicsItem::hoverMoveEvent(event);
}


void CCurveGraphicsItem::hoverLeaveEnvet(QGraphicsSceneHoverEvent *event)
{
    if (m_bEditFlag)
    {
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsItem::hoverLeaveEvent(event);
}


void CCurveGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_dragDir != NONE && m_bEditFlag)
    {
        m_startPos = event->scenePos();
    }

    QGraphicsItem::mousePressEvent(event);
}


void CCurveGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_dragDir == NONE)
        return  QGraphicsItem::mouseMoveEvent(event);

    if (!m_bEditFlag)
    {
        return  QGraphicsItem::mouseMoveEvent(event);
    }

    QPointF dis;
    dis = event->scenePos() - m_startPos;
    m_startPos = event->scenePos();

    switch (m_dragDir)
    {
    case NONE:
        break;
    case LEFT:
    {
        QRectF tempRect = QRectF(m_itemRectF.x(), m_itemRectF.y(), m_itemRectF.width() - dis.x(), m_itemRectF.height());
        this->resetItemSize(tempRect);
        this->moveBy(dis.x(), 0);
        break;
    }
    case RIGHT:
    {
        QRectF tempRect = QRectF(m_itemRectF.x(), m_itemRectF.y(), m_itemRectF.width() + dis.x(), m_itemRectF.height());
        this->resetItemSize(tempRect);
        this->moveBy(0, 0);
        scene()->update();
        break;
    }
    case UP:
    {
        QRectF tempRect = QRectF(m_itemRectF.x(), m_itemRectF.y(), m_itemRectF.width(), m_itemRectF.height() - dis.y());
        this->resetItemSize(tempRect);
        this->moveBy(0, dis.y());
        break;
    }
    case DOWN:
    {
        QRectF tempRect = QRectF(m_itemRectF.x(), m_itemRectF.y(), m_itemRectF.width(), m_itemRectF.height() + dis.y());
        this->resetItemSize(tempRect);
        this->moveBy(0, 0);
        scene()->update();
        break;
    }
    case LEFTTOP:
    {
        QRectF tempRect = QRectF(m_itemRectF.x(), m_itemRectF.y(), m_itemRectF.width() - dis.x(), m_itemRectF.height() - dis.y());
        this->resetItemSize(tempRect);
        this->moveBy(dis.x(), dis.y());
        break;
    }
    case RIGHTTOP:
    {
        QRectF tempRect = QRectF(m_itemRectF.x(), m_itemRectF.y(), m_itemRectF.width() + dis.x(), m_itemRectF.height() - dis.y());
        this->resetItemSize(tempRect);
        this->moveBy(0, dis.y());
        scene()->update();
        break;
    }
    case LEFTBOTTOM:
    {
        QRectF tempRect = QRectF(m_itemRectF.x(), m_itemRectF.y(), m_itemRectF.width() - dis.x(), m_itemRectF.height() + dis.y());
        this->resetItemSize(tempRect);
        this->moveBy(dis.x(), 0);
        break;
    }
    case RIGHTBOTTOM:
    {
        QRectF tempRect = QRectF(m_itemRectF.x(), m_itemRectF.y(), m_itemRectF.width() + dis.x(), m_itemRectF.height() + dis.y());
        this->resetItemSize(tempRect);
        this->moveBy(0, 0);
        scene()->update();
        break;
    }
    default:
        break;
    }
}


void CCurveGraphicsItem::keyPressEvent(QKeyEvent * event)
{
    if (m_bEditFlag)
    {
        return QGraphicsItem::keyPressEvent(event);
    }

    if (event->key() == Qt::Key_Plus)
    {
        if (m_dbXAxisMax > m_realXDefault*0.4)
        {
            m_dbXAxisMax -= m_dbXAxisMax*0.20;
           // m_iXTicksCount += m_iXTicksCount*0.5;
            update();
        }

    }
    else if (event->key() == Qt::Key_Minus)
    {
        
        if (m_dbXAxisMax < m_realXDefault*1.6)
        {
            m_dbXAxisMax += m_dbXAxisMax*0.20;
           // m_iXTicksCount -= m_iXTicksCount*0.5;
            update();
        }
    }
    
    QGraphicsItem::keyPressEvent(event);
}
