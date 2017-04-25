#include "das.h"

DAS::DAS(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    ui.setupUi(this);

    setWindowTitle(tr("DAS"));
    setWindowState(Qt::WindowMaximized);
    //setWindowIcon(QIcon(":/images/logo.png"));
}

DAS::~DAS()
{
}

