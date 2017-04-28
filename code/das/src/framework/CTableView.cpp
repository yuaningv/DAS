#include "CTableView.h"
#include "CFrozenTableView.h"

#include "QtWidgets/QHBoxLayout"
#include "QtGui/QStandardItemModel"
#include "QtWidgets/QHeaderView"
#include "QtCore/QEvent"
#include "QtGui/QMouseEvent"


CTableView::CTableView(QWidget* parent /*= 0*/)
    : CCustomWidgetBase(parent)
    , m_pTableView(nullptr)
    , m_pModel(nullptr)
    , m_strStartTime("")
    , m_strEndTime("")
{
    m_mapHorizontalHeader.clear();
    initLayout();
}

CTableView::~CTableView()
{

}


ITEMTYPE CTableView::type()
{
    return Item_Table;
}



// 设置水平表头数据 ，从xml文件读取后设置
void CTableView::setHorizontalHeader(QMap<QString, QString>& mapHeader)
{
    m_mapHorizontalHeader = mapHeader;
}

/*
* 设置开始结束时间
* strStart 开始时间 strEnd 结束时间  时间格式 yyyy/MM/dd hh:mm:ss
*/
void CTableView::setTimeRange(const QString& strStart, const QString& strEnd)
{
    m_strStartTime = strStart;
    m_strEndTime = strEnd;
}


// 初始化布局
void CTableView::initLayout()
{
    m_pModel = new QStandardItemModel(this);
    m_pModel->setColumnCount(m_mapHorizontalHeader.size());
    m_pTableView = new CFrozenTableView(m_pModel, 1);  // 首列冻结
    m_pTableView->resizeColumnsToContents();
    m_pTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_pTableView->setAlternatingRowColors(true);

    QHBoxLayout* pTmpLayout = new QHBoxLayout(this);
    pTmpLayout->addWidget(m_pTableView);

    this->setLayout(pTmpLayout);
}

