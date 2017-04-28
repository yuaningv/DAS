#include "CToolBar.h"

#include "QtGui/QDragEnterEvent"
#include "QtGui/QDragLeaveEvent"
#include "QtGui/QMouseEvent"
#include "QtWidgets/QAction"
#include "QtCore/QMimeData"
#include "QtWidgets/QApplication"
#include "QtGui/QDrag"
#include "QtWidgets/QLabel"


CToolBar::CToolBar(QWidget *parent)
    : QToolBar(parent)
    , m_bEditFlag(false)
{
    QApplication::setStartDragDistance(5);
    this->setAcceptDrops(true);
    setStyleSheet("QToolBar{ spacing: 10px; }");
}

CToolBar::~CToolBar()
{

}

void CToolBar::setEditModeEnabled(bool enable)
{ 
    m_bEditFlag = enable; 
}


void CToolBar::dragEnterEvent(QDragEnterEvent * event)
{
    if (!m_bEditFlag)
    {
        return QWidget::dragEnterEvent(event);
    }
    if (event->source() == this)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
    {
        event->acceptProposedAction();
    }

    QWidget::dragEnterEvent(event);
}

void CToolBar::mouseMoveEvent(QMouseEvent * event)
{
    if (!m_pChildAction || !m_bEditFlag)
    {
        return;
    }
    if ((event->buttons() & Qt::LeftButton) == Qt::LeftButton)
    {
        if ((event->pos() - m_startPos).manhattanLength()
            >= QApplication::startDragDistance())
        {
            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData();
            QByteArray data;
            QDataStream in(&data, QIODevice::WriteOnly);
            in << m_pChildAction->property("ItemType").toInt();
            mimeData->setData(QString("CustomWidget"), data);

            //QIcon tmpIcon = m_pChildAction->pixmap();
            QPixmap pixmap = *(m_pChildAction->pixmap());

            drag->setMimeData(mimeData);
            drag->setPixmap(pixmap);
            drag->setHotSpot(QPoint(0, 0)); //设置跟随图片的中心点

            Qt::DropAction dropAction = drag->exec();
            m_pChildAction = NULL;

            return;
        }
    }

    QWidget::mouseMoveEvent(event);
}

void CToolBar::mousePressEvent(QMouseEvent * event)
{
    if (event->button() != Qt::LeftButton || !m_bEditFlag)
        return QWidget::mousePressEvent(event);

    m_pChildAction = static_cast<QLabel*>(childAt(event->pos()));
    if (!m_pChildAction)
    {
        return;
    }
    m_startPos = event->pos();
}

