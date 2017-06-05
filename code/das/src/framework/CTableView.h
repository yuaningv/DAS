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
#include "QtCore/QMutex"
#include "QtWidgets/QLabel"

class CFrozenTableView;

QT_BEGIN_NAMESPACE
//class QStandardItemModel;
class QPushButton;
class QThread;
class QLabel;
QT_END_NAMESPACE

#include "constant.h"
#include "StreamMgr.h"
#include "CanDecoder.h"


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


class CTableView : public CCustomWidgetBase, public CStreamListener
{
    Q_OBJECT

public:
    explicit CTableView(QWidget* parent = 0);
    ~CTableView();

    void setHorizontalHeader(QList<CurveLine_t>& lstHeader);
    void insertRowData(QList<CurveLine_t>& lstRowData = QList<CurveLine_t>());

    void setTimeRange(const QString& strStart, const QString& strEnd);

    void setChannel(int iChannel) { m_iChannel = iChannel; };       // 通过channel来判断该组件是否已经关联数据，iChannel < 0无关联数据，iChannel >= 0关联数据 
    int getChannel() { return m_iChannel; };

    void setTitle(const QString& strTitle) { m_pLbTitle->setText(strTitle); }
    QString getTitle() const { return m_pLbTitle->text().trimmed(); }

private:
    void initLayout();

    void setTableViewData();

    // 创建表头
    void createHorizontalHeaders();

protected:
    virtual ITEMTYPE type();

    virtual HWND GetWndHandle();
    virtual void OnMedia(unsigned char* buffer, unsigned long length, unsigned long payload,
		CCustomDateTime* pTime, void* pCustomData);

private slots:
    void OnExport();

private:
    CFrozenTableView* m_pTableView;
    CTableModel* m_pModel;

    QString m_strStartTime;
    QString m_strEndTime;

    QPushButton* m_pExcelBtn;
    QLabel* m_pLbTitle;
    QList<CurveLine_t> m_lstHorizontalHeader;
    QThread* m_pThread;

    int m_iChannel;
    QMutex m_mutex;

};

#endif // CTABLEVIEW_H
