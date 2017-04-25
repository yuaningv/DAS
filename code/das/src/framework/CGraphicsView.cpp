#include "CGraphicsView.h"


CGraphicsView::CGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setCursor(Qt::ArrowCursor);
    setMouseTracking(false);
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    m_pScene = new CGraphicsScene(parent);

    setScene(m_pScene);
    setSceneRect(QRectF(0, 0, 20000, 20000));
    //setBackgroundBrush(QBrush(Qt::gray, Qt::Dense7Pattern));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setRenderHint(QPainter::Antialiasing, true);
}


CGraphicsView::~CGraphicsView()
{
    if (m_pScene != NULL)
    {
        delete m_pScene;
        m_pScene = NULL;
    }
}





