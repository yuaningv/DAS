#ifndef DAS_H
#define DAS_H

#include "QtWidgets/QMainWindow"
#include "QtCore/QTranslator"
#include "CGraphicsView.h"
#include "CPropertyBar.h"
#include "QtWidgets/QComboBox"

class CToolBar;
class QLabel;
class QCheckBox;
class CSelectedArea;

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
    virtual void keyReleaseEvent(QKeyEvent *ev);

private:
    QAction* m_pActOpen;
    QAction* m_pActExit;
    QAction* m_pActPlay;
    QAction* m_pActFind;
    QAction* m_pActFullScreen;
    QAction* m_pActZhCn;
    QAction* m_pActEn;
    QAction* m_pActAbout;
    QAction* m_pActScreenshot;
    QComboBox* m_pStepCombo;

    QLabel* m_pLbTimeAxis;
    QLabel* m_pLbVideo;
    QLabel* m_pLbCurve;
    QLabel* m_pLbTable;
    QCheckBox* m_pCBoxEdit;

    QMenu* m_pMenuFile;
    QMenu* m_pMenuEdit;
    QMenu* m_pMenuView;
    QMenu* m_pMenuTool;
    QMenu* m_pMenuLanuage;
    QMenu* m_pMenuHelp;

    QToolBar* m_pStandardToolBar;
    QToolBar* m_pOperatorToolBar;
    CToolBar* m_pModuleToolBar;

    QTranslator m_translator;
    CSelectedArea* m_pSelectedArea;  // 截屏区域 

    bool m_bPlay;       // 控制播放与暂停的flag 
private:
    CGraphicsView* m_pGraphicsView;
    CPropertyBar* m_pPropertyBar;

private slots:
    void OnOpen();
    void OnPlay();
    void OnReset();
    void OnFind();
    void OnFullScreen();
    void OnLanuageChanged(QAction* pAction);
    void OnAbout();

    void OnScreenshot();
    /*void OnPlayFast();
    void OnPlaySlow();*/
    void OnStepChanged(const QString& strStep);
    void OnScreenShotAreaSelected(const QRect& rect);

    void OnEditCheckBoxStateChanged(int state);

};

#endif // SMS_H
