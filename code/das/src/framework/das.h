#ifndef DAS_H
#define DAS_H

#include "QtWidgets/QMainWindow"
#include "QtCore/QTranslator"
#include "CGraphicsView.h"

class CToolBar;
class QLabel;

class DAS : public QMainWindow
{
    Q_OBJECT

public:
    DAS(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~DAS();

private:
    void setLayout();
    void retranslate();

protected:
    virtual void changeEvent(QEvent *ev);

private:
    QAction* m_pActOpen;
    QAction* m_pActExit;
    QAction* m_pActPlay;
    QAction* m_pActFind;
    QAction* m_pActFullScreen;
    QAction* m_pActZhCn;
    QAction* m_pActEn;
    QAction* m_pActAbout;

    QLabel* m_pLbTimeAxis;
    QLabel* m_pLbVideo;
    QLabel* m_pLbCurve;
    QLabel* m_pLbTable;

    QMenu* m_pMenuFile;
    QMenu* m_pMenuEdit;
    QMenu* m_pMenuView;
    QMenu* m_pMenuTool;
    QMenu* m_pMenuLanuage;
    QMenu* m_pMenuHelp;

    QToolBar* m_pOperatorToolBar;
    CToolBar* m_pModuleToolBar;

    QTranslator m_translator;

private:
    CGraphicsView* m_pGraphicsView;

    QTimer* m_pTimer;
    QWidget* m_pWidget;
    bool m_bFlag;

private slots:
    void OnOpen();
    void OnPlay();
    void OnFind();
    void OnFullScreen();
    void OnLanuageChanged(QAction* pAction);
    void OnAbout();

    void OnUpdate();
};

#endif // SMS_H
