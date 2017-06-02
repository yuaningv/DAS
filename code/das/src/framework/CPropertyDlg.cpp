#include "CPropertyDlg.h"
#include "constant.h"
//#include "CFileOperationManager.h"


#include "QtWidgets/QPushButton"
#include "QtWidgets/QVBoxLayout"
#include "QtWidgets/QFormLayout"
#include "QtWidgets/QDialogButtonBox"
#include "QtWidgets/QSpacerItem"
#include "QtCore/QDebug"

extern const char* cstOk;
extern const char* cstCancel;

const char* cstProperty = "Property";
const char* cstTitle = "Title";
const char* cstChannel = "Channel";


CPropertyDlg::CPropertyDlg(QWidget *parent /*= 0*/)
    : QDialog(parent)
    , m_pSBoxChannel(nullptr)
    , m_pLEditName(nullptr)
    , m_pBtnOk(nullptr)
    , m_pBtnCancel(nullptr)
{
    setWindowTitle(trFormString(cstProperty));
    setFixedSize(250, 200);
    //setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    initLayout();
}

CPropertyDlg::~CPropertyDlg()
{

}


void CPropertyDlg::initLayout()
{
    m_pLEditName = new QLineEdit(this);
    m_pSBoxChannel = new QSpinBox(this);
    m_pSBoxChannel->setMinimum(0);

    m_pBtnOk = new QPushButton(trFormString(cstOk), this);
    m_pBtnCancel = new QPushButton(trFormString(cstCancel), this);

    connect(m_pBtnOk, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_pBtnCancel, &QPushButton::clicked, this, &QDialog::reject);

    QFormLayout* pFormLayout = new QFormLayout(this);
    //pFormLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    //pFormLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    pFormLayout->setFormAlignment(Qt::AlignCenter);
    pFormLayout->setLabelAlignment(Qt::AlignRight);
    pFormLayout->setVerticalSpacing(20);

    pFormLayout->addRow(trFormString(cstTitle) + ":", m_pLEditName);
    pFormLayout->addRow(trFormString(cstChannel) + ":", m_pSBoxChannel);

    QDialogButtonBox* pDlgBtnBox = new QDialogButtonBox(this);
    pDlgBtnBox->addButton(m_pBtnOk, QDialogButtonBox::ActionRole);
    pDlgBtnBox->addButton(m_pBtnCancel, QDialogButtonBox::ActionRole);

    pFormLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Expanding));
    pFormLayout->addRow(pDlgBtnBox);

    setLayout(pFormLayout);
}
