#ifndef CPropertyBar_H
#define CPropertyBar_H

#include "QtWidgets/QDockWidget"
#include "QtWidgets/QMessageBox"

#include "QtWidgets/QTreeView.h"
#include "QtGui/QStandardItemModel.h"

#include "type.h"


class CTreeView : public QTreeView
{
private:
    QSize sizeHint() const  
    {  
        return QSize(200, baseSize().height()); // 在这里定义dock的初始大小  
    }
};

class CPropertyBar : public QDockWidget
{
    Q_OBJECT

public:
    CPropertyBar(QWidget *parent = 0);
    ~CPropertyBar();

private:

private:
    QTreeView           *m_pView;
    QStandardItemModel  *m_pModel;

private slots:
    void OnShowItemAttr(const ItemAttribute_t& itemAttr);
};

#endif // CPropertyBar_H
