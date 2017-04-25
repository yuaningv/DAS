#ifndef CGraphicsView_h
#define CGraphicsView_h

#include "QtWidgets/QGraphicsView"
#include "CGraphicsScene.h"


class CGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    CGraphicsView(QWidget *parent = 0);
    ~CGraphicsView();

private:
    CGraphicsScene* m_pScene;

signals:
};


#endif
