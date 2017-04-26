/*
* File    : CFrozenTableView.h
*
* Date    : 2017-04-25
*
* Author  : 
*
* Brief   : 自定义tableview 实现固定列功能
*
* Note    :
*/


#ifndef CFROZEN_TABLEVIEW_H
#define CFROZEN_TABLEVIEW_H


#include "QtWidgets/QTableView"

class CFrozenTableView : public QTableView
{
    Q_OBJECT

public:
    CFrozenTableView(QAbstractItemModel * pModel, int iNum = 1);
    ~CFrozenTableView();

protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
    void scrollTo(const QModelIndex & index, ScrollHint hint = EnsureVisible);

public:
    void   setDelegateForColumn(int column, QAbstractItemDelegate *delegate, bool bFlag);
    void   setFrozenColumn(const int iColumn);
    int    frozenColumn();

private:
    void init();
    void updateFrozenTableGeometry();


private:
    QTableView *m_pFrozenTableView;
    int m_iFrozenColumnNum;

    private slots:
    void updateSectionWidth(int logicalIndex, int, int newSize);
    void updateSectionHeight(int logicalIndex, int, int newSize);
};

#endif // CFROZEN_TABLEVIEW_H
