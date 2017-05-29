#include "CVideoWidget.h"

#include "QtGui/QKeyEvent"
#include "QtWidgets/QLabel"

#include "CLogManager.h"


CVideoWidget::CVideoWidget(QWidget* parent /*= 0*/)
    : CCustomWidgetBase(parent),
    m_bFullscreenFlag(false),
    m_bFlag(true),
    m_pVideoData(NULL),
    m_iChannel(0)
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

    m_pVideoData = new CVideoDataListener;
    connect(m_pVideoData, SIGNAL(sigVideoUpdate(const QImage&, const QDateTime&)), this, SLOT(OnUpdateFrame(const QImage&, const QDateTime&)));
}


CVideoWidget::~CVideoWidget()
{
    if (m_pVideoData != NULL)
    {
        delete m_pVideoData;
        m_pVideoData = NULL;
    }
}


void CVideoWidget::setTitle(const QString& strTitle)
{
    m_labelTitle->setText(strTitle);
    m_strTitle = strTitle;
}


int CVideoWidget::init(const QString& strStorage, int iChannel /* = -1 */)
{
    m_strStorage = strStorage;
    m_iChannel = iChannel;
    if (m_iChannel < 0)
    {
        return 0;
    }

    m_pVideoData->init(strStorage, iChannel);

    return 1;
}


void CVideoWidget::setChannel(int iChannel)
{
    m_iChannel = iChannel;
    if (m_iChannel < 0)
    {
        return;
    }

    m_pVideoData->init(m_strStorage, iChannel);
}


int CVideoWidget::setScape(const QDateTime& dtBegin, const QDateTime& dtEnd)
{
    if (m_iChannel < 0)
    {
        return 0;
    }

    m_pVideoData->setScape(dtBegin, dtEnd);

    return 1;
}


int CVideoWidget::skipTo(const QDateTime& dtPos)
{
    if (m_iChannel < 0)
    {
        return 0;
    }

    m_pVideoData->skipTo(dtPos);

    return 1;
}


int CVideoWidget::play()
{
    if (m_iChannel < 0)
    {
        return 0;
    }

    m_pVideoData->play();

    return 1;
}


int CVideoWidget::pause()
{
    if (m_iChannel < 0)
    {
        return 0;
    }

    m_pVideoData->pause();

    return 1;
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


void CVideoWidget::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev);

    return;

    QPainter painter;
    painter.begin(this);

    if (m_image.size().width() <= 0)
    {
    }

    QRect rect(0, 0, this->width(), this->height());
    painter.drawImage(rect, m_image);

    painter.end();
}


void CVideoWidget::OnUpdateFrame(const QImage& image, const QDateTime& currentDateTime)
{
    //qobject_cast<CVideoDataListener *>(sender());

    //m_labelTitle->setText("1111111");

    CLogManager::getInstance()->log(eLogDebug, "CVideoWidget::OnUpdateFrame", "update frame, hwnd:%d", winId());

    /*QPalette pal;
    pal.setBrush(QPalette::Background, QBrush(image));
    this->setPalette(pal);*/

    updateFrame();

    CLogManager::getInstance()->log(eLogDebug, "CVideoWidget::OnUpdateFrame", "update frame, hwnd:%d", winId());
}

