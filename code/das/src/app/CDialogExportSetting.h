#ifndef CDialogExportSetting_H
#define CDialogExportSetting_H

#include "QtWidgets/QDialog"
#include "QtCore/QDateTime"
#include "QtWidgets/QDateTimeEdit"
#include "QtWidgets/QSpinBox"


class CDialogExportSetting : public QDialog 
{
    Q_OBJECT

public:
    CDialogExportSetting(const QDateTime& dtBegin, const QDateTime& dtEnd,  QWidget *parent = 0);
    ~CDialogExportSetting();

    QDateTime getDtBegin();
    QDateTime getDtEnd();

    int getChannel() { return m_iChannel; };

private:
    QDateTime m_dtBegin;
    QDateTime m_dtEnd;
    QDateTimeEdit* m_pEditBegin;
    QDateTimeEdit* m_pEditEnd;
    QSpinBox* m_pSpinBoxChannel;

    int m_iChannel;

private slots:
    void OnOk();
    void OnCancel();
};

#endif // CDialogExportSetting_H
