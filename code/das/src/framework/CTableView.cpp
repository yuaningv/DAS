#include "CTableView.h"
#include "CFrozenTableView.h"
#include "CFileOperationManager.h"
#include "constant.h"

#include "QtWidgets/QVBoxLayout"
#include "QtGui/QStandardItemModel"
#include "QtWidgets/QHeaderView"
#include "QtCore/QEvent"
#include "QtGui/QMouseEvent"
#include "QtWidgets/QPushButton"
#include "QtWidgets/QFileDialog"
#include "QtWidgets/QMessageBox"

const char* cstExportSuccess = "Export Success!";
const char* cstExportFailed = "Export Failed!";
const char* cstTips = "Tips";
const char* cstExport = "Export excel";

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

    QVBoxLayout* pTmpLayout = new QVBoxLayout(this);
    pTmpLayout->addWidget(m_pTableView);

    m_pExcelBtn = new QPushButton(trFormString(cstExport), this);
    connect(m_pExcelBtn, &QPushButton::clicked, this, &CTableView::OnExport);

    pTmpLayout->addWidget(m_pExcelBtn, 0, Qt::AlignRight);

    this->setLayout(pTmpLayout);
}


// 填充表格数据
void CTableView::setTableViewData()
{

}

// 导出excel
void CTableView::OnExport()
{
    QString strFileName = QFileDialog::getSaveFileName(nullptr, trFormString(cstExport), "./", "*.xlsx");
    if (strFileName.isEmpty())
    {
        return;
    }

    if (QFileInfo(strFileName).suffix().isEmpty())
    {
        strFileName += ".xlsx";
    }

    // 表头
   // m_mapHorizontalHeader["aa"] = "aa";
    //m_mapHorizontalHeader["bb"] = "bb";

    QStringList slstHeader;
    for (auto& itr = m_mapHorizontalHeader.begin(); itr != m_mapHorizontalHeader.end(); ++itr)
    {
        slstHeader << itr.value();
    }

    // 数据源
    QList<QList<QVariant> > lstData;
    //lstData.append(QList<QVariant>() << "aa" << "bb");
    CFileOperationManager oTmpFileMananger;
    if (!oTmpFileMananger.writeExcelFile(strFileName, slstHeader, lstData))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle(trFormString(cstTips));
        msgBox.setText(trFormString(cstExportFailed));
        msgBox.exec();
        return;
    }

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(trFormString(cstTips));
    msgBox.setText(trFormString(cstExportSuccess));
    msgBox.exec();
}
