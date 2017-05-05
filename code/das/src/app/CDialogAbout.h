#ifndef CDIALOGABOUT_H
#define CDIALOGABOUT_H

#include "QtWidgets/QDialog"


class CDialogAbout : public QDialog 
{
    Q_OBJECT

public:
    CDialogAbout(QWidget *parent = 0);
    ~CDialogAbout();

private:
};

#endif // CDIALOGABOUT_H
