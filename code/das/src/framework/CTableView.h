/*
* File    : CTableView.h
*
* Date    : 2017-04-25
*
* Author  : 
*
* Brief   : table表控件
*
* Note    :
*/


#ifndef CTABLEVIEW_H
#define CTABLEVIEW_H


#include "CCustomWidgetBase.h"
#include "QtCore/QMap"
#include "QtGui/QStandardItemModel"

class CFrozenTableView;

QT_BEGIN_NAMESPACE
//class QStandardItemModel;
class QPushButton;
class QThread;
QT_END_NAMESPACE

class CTableModel : public QStandardItemModel
{
public:
    CTableModel(QObject * parent = 0)
        : QStandardItemModel(parent)
    {

    }

    ~CTableModel()
    {

    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
    {
        if (!index.isValid())
        {
            return QVariant();
        }

        if (role == Qt::TextAlignmentRole)
        {
            return int(Qt::AlignCenter);
        }
       
        return QStandardItemModel::data(index, role);
    }

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        if (!index.isValid())
            return Qt::NoItemFlags;

        Qt::ItemFlags flag = QStandardItemModel::flags(index);

        flag &= ~Qt::ItemIsEditable;

        return flag;
    }
private:

};


class CTableView : public CCustomWidgetBase
{
    Q_OBJECT

public:
    explicit CTableView(QWidget* parent = 0);
    ~CTableView();

    void setHorizontalHeader(QList<CurveLine_t>& lstHeader);
    void insertRowData(QList<CurveLine_t>& lstRowData = QList<CurveLine_t>());

    void setTimeRange(const QString& strStart, const QString& strEnd);

private:
    void initLayout();

    void setTableViewData();

    // 创建表头
    void createHorizontalHeaders();

protected:
    virtual ITEMTYPE type();

private slots:
    void OnExport();


private:
    CFrozenTableView* m_pTableView;
    CTableModel* m_pModel;

    QString m_strStartTime;
    QString m_strEndTime;

    QPushButton* m_pExcelBtn;
    QList<CurveLine_t> m_lstHorizontalHeader;
    QThread* m_pThread;
};

#endif // CTABLEVIEW_H
