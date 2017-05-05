#include "CConfigCurveDlg.h"
#include "constant.h"

#include "QtWidgets/QListWidget"
#include "QtWidgets/QListWidgetItem"
#include "QtWidgets/QPushButton"
#include "QtWidgets/QVBoxLayout"
#include "QtWidgets/QDialogButtonBox"
#include "QtCore/QDebug"

const char* cstSelectCurve = "Select Curve";
const char* cstOk = "Ok";
const char* cstCancel = "Cancel";

CConfigCurveDlg::CConfigCurveDlg(QWidget *parent /*= 0*/)
    : QDialog(parent)
    , m_pListWidget(nullptr)
    , m_pBtnOk(nullptr)
    , m_pBtnCancel(nullptr)
{
    setWindowTitle(trFormString(cstSelectCurve));
    setMaximumWidth(400);

    initLayout();
}

CConfigCurveDlg::~CConfigCurveDlg()
{

}


void CConfigCurveDlg::setCheckedLines(const QList<CurveLine_t>& lstTmpData) 
{ 
    m_lstCheckedLine = lstTmpData;
    createListWidgetData();
}


void CConfigCurveDlg::initLayout()
{
    m_pListWidget = new QListWidget(this);

    m_pBtnOk = new QPushButton(trFormString(cstOk), this);
    m_pBtnCancel = new QPushButton(trFormString(cstCancel), this);

    connect(m_pBtnOk, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_pBtnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_pListWidget, &QListWidget::itemChanged, this, &CConfigCurveDlg::OnItemChanged);

    QDialogButtonBox* pDlgBtnBox = new QDialogButtonBox(this);
    pDlgBtnBox->addButton(m_pBtnOk, QDialogButtonBox::ActionRole);
    pDlgBtnBox->addButton(m_pBtnCancel, QDialogButtonBox::ActionRole);

    QVBoxLayout* pLayoutV = new QVBoxLayout(this);
    pLayoutV->addWidget(m_pListWidget);
    pLayoutV->addWidget(pDlgBtnBox);

    setLayout(pLayoutV);
}


void CConfigCurveDlg::createListWidgetData()
{
    // 读取配置文件中所有项
    QList<CurveLine_t> lstLine;
    CurveLine_t tmpLine;
    tmpLine.m_strName = "aa";
    tmpLine.m_color = Qt::red;
    lstLine << tmpLine;
    tmpLine.m_strName = "bb";
    tmpLine.m_color = Qt::blue;
    tmpLine.m_realMin = 0.0;
    tmpLine.m_realMax = 5.0;
    lstLine << tmpLine;
    tmpLine.m_strName = "cc";
    tmpLine.m_color = Qt::green;
    tmpLine.m_realMin = 1.0;
    tmpLine.m_realMax = 10.0;
    lstLine << tmpLine;

    for (auto& TmpData : lstLine)
    {
        QListWidgetItem* pItem = new QListWidgetItem(TmpData.m_strName);
        pItem->setData(CurveRole, QVariant::fromValue(TmpData));
        pItem->setBackground(QBrush(TmpData.m_color));
        if (m_lstCheckedLine.contains(TmpData))
        {
            pItem->setCheckState(Qt::Checked);
        }
        else
        {
            pItem->setCheckState(Qt::Unchecked);
        }
        m_pListWidget->addItem(pItem);
    }
}


void CConfigCurveDlg::OnItemChanged(QListWidgetItem* item)
{
    qDebug() << "item changed";
    if (Qt::Checked == item->checkState())
    {
        m_lstCheckedLine.append(item->data(CurveRole).value<CurveLine_t>());  
    }
    else
    {
        if (m_lstCheckedLine.contains(item->data(CurveRole).value<CurveLine_t>()))
        {
            m_lstCheckedLine.removeAll(item->data(CurveRole).value<CurveLine_t>());
        }  
    }

    if (m_lstCheckedLine.size() < 5)   // 最多可选5条线
    {
        for (int i = 0; i < m_pListWidget->count(); ++i)
        {
            m_pListWidget->blockSignals(true);
            QListWidgetItem* pTmpItem = m_pListWidget->item(i);
            pTmpItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
            m_pListWidget->blockSignals(false);
        }
    }
    else
    {
        for (int i = 0; i < m_pListWidget->count(); ++i)
        {
            QListWidgetItem* pTmpItem = m_pListWidget->item(i);
            if (Qt::Checked != pTmpItem->checkState())
            {
                m_pListWidget->blockSignals(true);
                pTmpItem->setFlags(Qt::ItemIsUserCheckable);
                m_pListWidget->blockSignals(false);
            }
        }
    }
}
