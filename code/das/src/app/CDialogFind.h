#ifndef CDialogFind_H
#define CDialogFind_H

#include "QtWidgets/QDialog"
#include "QtCore/QDateTime"
#include "QtWidgets/QDateTimeEdit"


class CDialogFind : public QDialog 
{
    Q_OBJECT

public:
    CDialogFind(const QDateTime& dtBegin, const QDateTime& dtEnd,  QWidget *parent = 0);
    ~CDialogFind();

    QDateTime getDtBegin();
    QDateTime getDtEnd();

private:
    QDateTime m_dtBegin;
    QDateTime m_dtEnd;
    QDateTimeEdit* m_pEditBegin;
    QDateTimeEdit* m_pEditEnd;

private slots:
    void OnOk();
    void OnCancel();
};

#endif // CDialogFind_H
