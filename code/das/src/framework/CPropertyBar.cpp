#include "CPropertyBar.h"
#include "constant.h"


// 定义多国语关键字常量 
const char* cstAttributeBar = "Attribute bar";
const char* cstAttribute = "Attribute";
const char* cstAttributeValue = "Attribute value";


CPropertyBar::CPropertyBar(QWidget *parent)
    : QDockWidget(parent),
    m_pView(NULL),
    m_pModel(NULL)
{
    setWindowTitle(trFormString(cstAttributeBar));

    m_pModel = new QStandardItemModel;
    m_pView = new CTreeView;
    m_pView->setRootIsDecorated(false);
    m_pView->setAlternatingRowColors(true);
    m_pView->setModel(m_pModel);
    //m_pView->setEditTriggers(QAbstractItemView::DoubleClicked);
    //m_pView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pView->setStyleSheet("QTreeView::item{height:25px}");

    m_pModel->setHorizontalHeaderLabels(QStringList() << trFormString(cstAttribute) << trFormString(cstAttributeValue));
   // connect(m_pModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(OnItemDataChanged(QStandardItem*)));
    setWidget(m_pView);
}


CPropertyBar::~CPropertyBar()
{
}


void CPropertyBar::OnShowItemAttr(const ItemAttribute_t& itemAttr)
{
    m_pModel->removeRows(0, m_pModel->rowCount());          // 清空上一次属性
    m_pModel->setRowCount(8);

    m_pModel->setData(m_pModel->index(0, 0), trFormString("ID"));
    m_pModel->setData(m_pModel->index(0, 1), itemAttr.iID);
    m_pModel->setData(m_pModel->index(1, 0), trFormString("Type"));
    m_pModel->setData(m_pModel->index(1, 1), itemAttr.iType);
    m_pModel->setData(m_pModel->index(2, 0), trFormString("Title"));
    m_pModel->setData(m_pModel->index(2, 1), itemAttr.strTitle);
    m_pModel->setData(m_pModel->index(3, 0), trFormString("Channel"));
    m_pModel->setData(m_pModel->index(3, 1), itemAttr.iChannel);
    m_pModel->setData(m_pModel->index(4, 0), trFormString("X"));
    m_pModel->setData(m_pModel->index(4, 1), itemAttr.iPosX);
    m_pModel->setData(m_pModel->index(5, 0), trFormString("Y"));
    m_pModel->setData(m_pModel->index(5, 1), itemAttr.iPoxY);
    m_pModel->setData(m_pModel->index(6, 0), trFormString("Width"));
    m_pModel->setData(m_pModel->index(6, 1), itemAttr.iWidth);
    m_pModel->setData(m_pModel->index(7, 0), trFormString("Height"));
    m_pModel->setData(m_pModel->index(7, 1), itemAttr.iHeight);

    m_pModel->item(0, 0)->setEnabled(false);
    m_pModel->item(1, 0)->setEnabled(false);
    m_pModel->item(2, 0)->setEnabled(false);
    m_pModel->item(3, 0)->setEnabled(false);
    m_pModel->item(4, 0)->setEnabled(false);
    m_pModel->item(5, 0)->setEnabled(false);
    m_pModel->item(6, 0)->setEnabled(false);
    m_pModel->item(7, 0)->setEnabled(false);
}


