#include "CDialogFind.h"
#include "constant.h"

#include "QtWidgets/QPushButton"
#include "QtWidgets/QDialogButtonBox"
#include "QtWidgets/QVBoxLayout"
#include "QtWidgets/QLabel"

extern const char* cstDictOK;
extern const char* cstDictFind;
const char* cstDictCancel = "Cancel";
const char* cstDictBeginTime = "Begin time";       // 开始时间 
const char* cstDictEndTime = "End time";           // 结束时间 


CDialogFind::CDialogFind(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(trFormString(cstDictFind));
    resize(300, 150);

    m_dtBegin = QDateTime::currentDateTime();
    m_dtEnd = m_dtBegin;

    QLabel* pTimeBeginLabel = new QLabel(this);
    pTimeBeginLabel->setText(trFormString(cstDictBeginTime));
    pTimeBeginLabel->setFixedWidth(100);

    m_pEditBegin = new QDateTimeEdit(this);
    m_pEditBegin->setDateTime(m_dtBegin);

    QLabel* pTimeEndLabel = new QLabel(this);
    pTimeEndLabel->setText(trFormString(cstDictEndTime));
    pTimeEndLabel->setFixedWidth(100);

    m_pEditEnd = new QDateTimeEdit(this);
    m_pEditEnd->setDateTime(m_dtEnd);

    QPushButton* pOkBtn = new QPushButton(trFormString(cstDictOK));
    pOkBtn->setDefault(true);
    connect(pOkBtn, SIGNAL(clicked()), this, SLOT(OnOk()));

    QPushButton* pCancelBtn = new QPushButton(trFormString(cstDictCancel));
    pCancelBtn->setCheckable(true);
    pCancelBtn->setAutoDefault(false);
    connect(pCancelBtn, SIGNAL(clicked()), this, SLOT(OnCancel()));

    QDialogButtonBox* pBtnBox = new QDialogButtonBox(Qt::Horizontal);
    pBtnBox->addButton(pOkBtn, QDialogButtonBox::ActionRole);
    pBtnBox->addButton(pCancelBtn, QDialogButtonBox::ActionRole);

    QGridLayout *pLayout = new QGridLayout(this);
    pLayout->addWidget(pTimeBeginLabel, 0, 0);
    pLayout->addWidget(m_pEditBegin, 0, 1);
    pLayout->addWidget(pTimeEndLabel, 1, 0);
    pLayout->addWidget(m_pEditEnd, 1, 1);
    pLayout->addWidget(pBtnBox, 2, 0, 1, 2);
    setLayout(pLayout);
}


CDialogFind::~CDialogFind()
{
}


QDateTime CDialogFind::getDtBegin()
{
    return m_dtBegin;
}


QDateTime CDialogFind::getDtEnd()
{
    return m_dtEnd;
}


void CDialogFind::OnOk()
{
    m_dtBegin = m_pEditBegin->dateTime();
    m_dtEnd = m_pEditEnd->dateTime();

    this->accept();
}

void CDialogFind::OnCancel()
{
    this->reject();
}
