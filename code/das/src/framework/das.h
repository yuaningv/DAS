#ifndef DAS_H
#define DAS_H

#include "QtWidgets/QMainWindow"
#include "ui_das.h"


class DAS : public QMainWindow
{
    Q_OBJECT

public:
    DAS(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~DAS();

private:
    Ui::DASClass               ui;

};

#endif // SMS_H
