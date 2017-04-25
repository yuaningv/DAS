#ifndef DAS_H
#define DAS_H

#include "QtWidgets/QMainWindow"
#include "QtCore/QTranslator"
#include "CGraphicsView.h"


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
    QAction* m_pActTimeline;
    QAction* m_pActVideo;
    QAction* m_pActCurve;
    QAction* m_pActTable;

    QMenu* m_pMenuFile;
    QMenu* m_pMenuEdit;
    QMenu* m_pMenuView;
    QMenu* m_pMenuTool;
    QMenu* m_pMenuLanuage;
    QMenu* m_pMenuHelp;

    QToolBar* m_pOperatorToolBar;
    QToolBar* m_pModuleToolBar;

    QTranslator m_translator;

private:
    CGraphicsView* m_pGraphicsView;

private slots:
    void OnOpen();
    void OnPlay();
    void OnFind();
    void OnFullScreen();
    void OnLanuageChanged(QAction* pAction);
    void OnAbout();
};

#endif // SMS_H
