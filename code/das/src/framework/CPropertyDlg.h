/*
* File    : CPropertyDlg.h
*
* Date    : 2017-06-01
*
* Author  : 
*
* Brief   :  Ù–‘≈‰÷√øÚ
*
* Note    :
*/


#ifndef CPROPERTY_DLG_H
#define CPROPERTY_DLG_H


#include <QtWidgets/QDialog>
#include "QtWidgets/QSpinBox"
#include "QtWidgets/QLineEdit"

QT_BEGIN_NAMESPACE
class QPushButton;
//class QSpinBox;
//class QLineEdit;
QT_END_NAMESPACE

class CPropertyDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CPropertyDlg(QWidget *parent = 0);
    ~CPropertyDlg();

    void setName(const QString& strName) { m_pLEditName->setText(strName); }
    void setChannel(const int& iChannel) { m_pSBoxChannel->setValue(iChannel); }
    QString getName() const { return m_pLEditName->text().trimmed(); }
    int getChannel() const { return m_pSBoxChannel->value(); }

private:
    void initLayout();

private slots:

private:
    QSpinBox* m_pSBoxChannel;
    QLineEdit* m_pLEditName;

    QPushButton* m_pBtnOk;
    QPushButton* m_pBtnCancel;

};

#endif 
