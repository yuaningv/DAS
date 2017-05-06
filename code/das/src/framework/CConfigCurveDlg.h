/*
* File    : CConfigCurveDlg.h
*
* Date    : 2017-05-4
*
* Author  : 
*
* Brief   : 选择显示曲线对话框
*
* Note    :
*/


#ifndef CCONFIG_CURVE_DLG_H
#define CCONFIG_CURVE_DLG_H


#include <QtWidgets/QDialog>
#include "type.h"

QT_BEGIN_NAMESPACE
class QListWidget;
class QPushButton;
class QListWidgetItem;
QT_END_NAMESPACE

class CConfigCurveDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CConfigCurveDlg(QWidget *parent = 0);
    ~CConfigCurveDlg();

    void setCheckedLines(const QList<CurveLine_t>& lstTmpData);

    QList<CurveLine_t> getCheckedLines()const;

private:
    void initLayout();
    void createListWidgetData();
    void setLinesColor();

private slots:
    void OnItemChanged(QListWidgetItem* item);

private:
    QListWidget* m_pListWidget;
    QPushButton* m_pBtnOk;
    QPushButton* m_pBtnCancel;

    QList<CurveLine_t> m_lstCheckedLine;   // 选中的线
    QList<QColor> m_lstColor;
};

#endif // CCONFIG_CURVE_DLG_H
