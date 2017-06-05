#include "CDialogAbout.h"
#include "version.h"
#include "constant.h"
#include "image.h"

#include "QtWidgets/QLabel"
#include "QtWidgets/QPushButton"

extern const char* cstDictAbout;
const char* cstDictZhuAnCompany = "Shanghai ZhuAn Intelligent Technology Co., Ltd";
const char* cstDictOK = "OK";


CDialogAbout::CDialogAbout(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(trMenuString(cstDictAbout));
    setFixedSize(450, 250);

    QLabel* pLblLogo = new QLabel(this);
    pLblLogo->setGeometry(20, 20, 80, 80);

    QLabel* pLblVersion = new QLabel(this);
    pLblVersion->setGeometry(120, 40, 330, 30);
    QFont font(font());
    font.setBold(true);
    pLblVersion->setFont(font);

    QLabel* pLblCopyright = new QLabel(this);
    pLblCopyright->setGeometry(20, 130, 420, 30);

    pLblVersion->setText(QString("%1_%2_%3_%4").arg(PRODUCT_NAME).arg(PRODUCT_VERSION).arg(PRODUCT_DATETIME).arg(PRODUCT_GREEK_VERSION));
    pLblCopyright->setText(QString("Copyright (C) 2017 ") + trFormString(cstDictZhuAnCompany));

    QPalette palBg;
    palBg.setBrush(QPalette::Window, QPixmap(IMG_LOGO).scaled(QSize(80, 80)));
    pLblLogo->setAutoFillBackground(true);
    pLblLogo->setPalette(palBg);

    QPushButton* pBtn = new QPushButton(this);
    pBtn->setGeometry(350, 210, 75, 25);
    pBtn->setText(trFormString(cstDictOK));
    connect(pBtn, SIGNAL(clicked()), this, SLOT(close()));
}


CDialogAbout::~CDialogAbout()
{
}

