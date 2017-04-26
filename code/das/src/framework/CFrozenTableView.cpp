#include "CFrozenTableView.h"

#include "QtWidgets/QHeaderView"
#include "QtWidgets/QScrollBar"


CFrozenTableView::CFrozenTableView(QAbstractItemModel *pModel, int iNum)
    : m_iFrozenColumnNum(iNum)
{
    setModel(pModel);
    m_pFrozenTableView = new QTableView(this);

    init();

    //设置tableView获取焦点在左上角
   /* QModelIndex index = pModel->index(0, 0);
    m_pFrozenTableView->setFocus();
    setCurrentIndex(index);*/

    //connect the headers and scrollbars of both tableviews together
    connect(horizontalHeader(), SIGNAL(sectionResized(int, int, int)), this, SLOT(updateSectionWidth(int, int, int)));
    connect(verticalHeader(), SIGNAL(sectionResized(int, int, int)), this, SLOT(updateSectionHeight(int, int, int)));

    connect(m_pFrozenTableView->verticalScrollBar(), SIGNAL(valueChanged(int)), verticalScrollBar(), SLOT(setValue(int)));
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), m_pFrozenTableView->verticalScrollBar(), SLOT(setValue(int)));


    connect(m_pFrozenTableView->horizontalHeader(), SIGNAL(sectionResized(int, int, int)), this, SLOT(updateSectionWidth(int, int, int)));

    connect(m_pFrozenTableView, SIGNAL(doubleClicked(const QModelIndex&)), this, SIGNAL(doubleClicked(const QModelIndex&)));

}


CFrozenTableView::~CFrozenTableView()
{
    delete m_pFrozenTableView;
}


void CFrozenTableView::init()
{
    m_pFrozenTableView->setModel(model());
    m_pFrozenTableView->setFocusPolicy(Qt::NoFocus);
    m_pFrozenTableView->verticalHeader()->hide();
    //   m_pFrozenTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    m_pFrozenTableView->setSortingEnabled(true);
    m_pFrozenTableView->resizeColumnsToContents();
    //m_pFrozenTableView->setAlternatingRowColors(true);
    m_pFrozenTableView->verticalHeader()->setDefaultSectionSize(25);

    viewport()->stackUnder(m_pFrozenTableView);

    m_pFrozenTableView->setStyleSheet("QTableView { border: none;"
        "background-color: #8EDE21;"
        "selection-background-color: #999}");
    m_pFrozenTableView->setSelectionModel(selectionModel());

    // 隐藏的列
    for (int iCol = m_iFrozenColumnNum; iCol < model()->columnCount(); iCol++)
    {
        m_pFrozenTableView->setColumnHidden(iCol, true);
    }

    // 显示的列
    for (int iCol = 0; iCol < m_iFrozenColumnNum; iCol++)
    {
        m_pFrozenTableView->setColumnWidth(iCol, columnWidth(iCol));
    }

    m_pFrozenTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pFrozenTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pFrozenTableView->show();

    updateFrozenTableGeometry();

    /// 默认QAbstractItemView::ScrollPerItem 
    //setHorizontalScrollMode(ScrollPerPixel);
    //setVerticalScrollMode(ScrollPerPixel);
    //m_pFrozenTableView->setVerticalScrollMode(ScrollPerPixel);
}


void CFrozenTableView::updateSectionWidth(int logicalIndex, int, int newSize)
{
    if (newSize <= 0)           // 固定列设置改变时,不改变非固定view的列宽
    {
        m_pFrozenTableView->setColumnWidth(logicalIndex, newSize);
        updateFrozenTableGeometry();
        return;
    }

    setColumnWidth(logicalIndex, newSize);
    m_pFrozenTableView->setColumnWidth(logicalIndex, newSize);
    updateFrozenTableGeometry();
}


void CFrozenTableView::updateSectionHeight(int logicalIndex, int, int newSize)
{
    m_pFrozenTableView->setRowHeight(logicalIndex, newSize);
}


void CFrozenTableView::resizeEvent(QResizeEvent * event)
{
    QTableView::resizeEvent(event);
    updateFrozenTableGeometry();

    int iStep = this->verticalScrollBar()->pageStep();
}


QModelIndex CFrozenTableView::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    QModelIndex current = QTableView::moveCursor(cursorAction, modifiers);

    if (cursorAction == MoveLeft && current.column() > 0
        && visualRect(current).topLeft().x() < m_pFrozenTableView->columnWidth(0))
    {
        const int newValue
            = horizontalScrollBar()->value() + visualRect(current).topLeft().x() - m_pFrozenTableView->columnWidth(0);
        horizontalScrollBar()->setValue(newValue);
    }

    return current;
}


void CFrozenTableView::scrollTo(const QModelIndex & index, ScrollHint hint)
{
    if (index.column() > 0)
    {
        QTableView::scrollTo(index, hint);
    }
}


void CFrozenTableView::setDelegateForColumn(int column, QAbstractItemDelegate *delegate, bool bFlag)
{
    if (bFlag)
    {
        setItemDelegateForColumn(column, delegate);
        m_pFrozenTableView->setItemDelegateForColumn(column, delegate);
    }
}

void CFrozenTableView::setFrozenColumn(const int iColumn)
{
    m_iFrozenColumnNum = iColumn;
    // 隐藏的列
    for (int iCol = m_iFrozenColumnNum; iCol < model()->columnCount(); iCol++)
    {
        m_pFrozenTableView->setColumnHidden(iCol, true);
    }

    // 显示的列
    for (int iCol = 0; iCol < m_iFrozenColumnNum; iCol++)
    {
        m_pFrozenTableView->setColumnHidden(iCol, false);
        //  m_pFrozenTableView->setColumnWidth(iCol, columnWidth(iCol));
    }
}

int CFrozenTableView::frozenColumn()
{
    return m_iFrozenColumnNum;
}


void CFrozenTableView::updateFrozenTableGeometry()
{
    int iWidth = 0;
    for (int iCol = 0; iCol < m_iFrozenColumnNum; iCol++)
    {
        iWidth += columnWidth(iCol);
    }

    m_pFrozenTableView->setGeometry(verticalHeader()->width() + frameWidth(),
        frameWidth(), iWidth, viewport()->height() + horizontalHeader()->height());
}

