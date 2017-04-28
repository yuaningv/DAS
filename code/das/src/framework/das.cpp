#include "das.h"
#include "constant.h"
#include "image.h"
#include "type.h"
#include "CToolBar.h"

#include "QtWidgets/QMenu"
#include "QtWidgets/QMenuBar"
#include "QtWidgets/QToolBar"
#include "QtWidgets/QStatusBar"
#include "QtWidgets/QLabel"


// 定义多国语关键字常量 
const char* cstDictDas = "DAS";

const char* cstFile = "File";
const char* cstEdit = "Edit";
const char* cstView = "View";
const char* cstTool = "Tool";
const char* cstHelp = "Help";

const char* cstDictOpenPath = "Open source data path";
const char* cstDictExit = "Exit system";
const char* cstDictPlay = "Play";
const char* cstDictFind = "Find";
const char* cstDictFullScreen = "Full screen";

const char* cstDictLanuage = "Lanuage";
//const char* cstDictZhCn = "Simplified chinese";
//const char* cstDictEn = "English";
const char* cstDictAbout = "About";

const char* cstOperatorToolBar = "Operator tool bar";
const char* cstModuleToolBar = "Module tool bar";

const char* cstDictTimelineModule = "Timeline";
const char* cstDictVideoModule = "Video";
const char* cstDictCurveModule = "Curve";
const char* cstDictTableModule = "Table";


DAS::DAS(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags),
    m_pGraphicsView(NULL),
    m_pPropertyBar(NULL)
{
    setWindowTitle(trFormString(cstDictDas));
    setWindowState(Qt::WindowMaximized);
    setWindowIcon(QIcon(IMG_LOGO));
    resize(800, 500);

    setLayout();

    m_pPropertyBar = new CPropertyBar(this);
    addDockWidget(Qt::RightDockWidgetArea, m_pPropertyBar);

    m_pGraphicsView = new CGraphicsView(this);
    this->setCentralWidget(m_pGraphicsView);

    connect(m_pGraphicsView, SIGNAL(sigItemAttr(const ItemAttribute_t&)), m_pPropertyBar, SLOT(OnShowItemAttr(const ItemAttribute_t&)));
}


DAS::~DAS()
{
    if (m_pGraphicsView != NULL)
    {
        delete m_pGraphicsView;
        m_pGraphicsView = NULL;
    }

    if (m_pPropertyBar != NULL)
    {
        delete m_pPropertyBar;
        m_pPropertyBar = NULL;
    }
}



void DAS::setLayout()
{
    // 创建文件菜单、创建文件菜单事件 
    // 打开数据源路径 
    m_pActOpen = new QAction(trMenuString(cstDictOpenPath), this);
    m_pActOpen->setIcon(QIcon(IMG_OPEN));
    m_pActOpen->setShortcut(QKeySequence("Ctrl+O"));
    //m_pActOpen->setStatusTip("");
    connect(m_pActOpen, SIGNAL(triggered()), this, SLOT(OnOpen()));

    // 退出系统 
    m_pActExit = new QAction(trMenuString(cstDictExit), this);
    m_pActExit->setIcon(QIcon(IMG_EXIT));
    m_pActExit->setShortcut(QKeySequence("Alt+F4"));
    connect(m_pActExit, SIGNAL(triggered()), this, SLOT(exit()));

    m_pMenuFile = new QMenu(trMenuString(cstFile), this);
    m_pMenuFile->addAction(m_pActOpen);
    m_pMenuFile->addAction(m_pActExit);

    // 创建编辑菜单、创建编辑菜单事件 
    m_pActPlay = new QAction(trMenuString(cstDictPlay), this);    // 播放、暂停、停止，三相开关 
    m_pActPlay->setIcon(QIcon(IMG_PLAY));
    m_pActPlay->setShortcut(QKeySequence("Ctrl+F5"));
    connect(m_pActPlay, SIGNAL(triggered()), this, SLOT(OnPlay()));

    m_pActFind = new QAction(trMenuString(cstDictFind), this);    // 查找 
    m_pActFind->setIcon(QIcon(IMG_FIND));
    m_pActFind->setShortcut(QKeySequence("Ctrl+F"));
    connect(m_pActFind, SIGNAL(triggered()), this, SLOT(OnFind()));

    m_pMenuEdit = new QMenu(trMenuString(cstEdit), this);
    m_pMenuEdit->addAction(m_pActFind);

    // 创建视图菜单、创建视图事件 
    m_pActFullScreen = new QAction(trMenuString(cstDictFullScreen), this);   // 全屏 
    m_pActFullScreen->setIcon(QIcon(IMG_FULLSCREEN));
    m_pActFullScreen->setShortcut(QKeySequence("Alt+Enter"));
    connect(m_pActFullScreen, SIGNAL(triggered()), this, SLOT(OnFullScreen()));

    m_pMenuView = new QMenu(trMenuString(cstView), this);
    m_pMenuView->addAction(m_pActFullScreen);

    // 创建工具菜单、创建工具菜单事件 
    m_pActZhCn = new QAction(QStringLiteral("简体中文"), this);   // 简体中文 
    m_pActZhCn->setIcon(QIcon(IMG_ENGLISH));
    m_pActZhCn->setCheckable(true);
    m_pActZhCn->setChecked(true);
    m_pActEn = new QAction(QStringLiteral("English"), this);       // 英语 
    m_pActEn->setIcon(QIcon(IMG_CHINESE));
    m_pActEn->setCheckable(true);

    m_pMenuTool = new QMenu(trMenuString(cstTool), this);
    m_pMenuTool->addAction(m_pActPlay);
    m_pMenuLanuage = new QMenu(trMenuString(cstDictLanuage), m_pMenuTool);
    connect(m_pMenuLanuage, SIGNAL(triggered(QAction*)), this, SLOT(OnLanuageChanged(QAction*)));
    m_pMenuLanuage->addAction(m_pActZhCn);
    m_pMenuLanuage->addAction(m_pActEn);
    m_pMenuTool->addAction(m_pMenuLanuage->menuAction());

    // 创建关于菜单、创建关于事件 
    m_pActAbout = new QAction(trMenuString(cstDictAbout), this);   // 关于 
    m_pActAbout->setIcon(QIcon(IMG_ABOUT));
    connect(m_pActAbout, SIGNAL(triggered()), this, SLOT(OnAbout()));

    m_pMenuHelp = new QMenu(trMenuString(cstHelp), this);
    m_pMenuHelp->addAction(m_pActAbout);

    QMenuBar* pMenuBar = new QMenuBar(this);
    pMenuBar->addMenu(m_pMenuFile);
    pMenuBar->addMenu(m_pMenuEdit);
    pMenuBar->addMenu(m_pMenuView);
    pMenuBar->addMenu(m_pMenuTool);
    pMenuBar->addMenu(m_pMenuHelp);
    this->setMenuBar(pMenuBar);

    // 创建工具栏(操作工具栏、组件工具栏）  
    m_pOperatorToolBar = this->addToolBar(trMenuString(cstOperatorToolBar));
    m_pOperatorToolBar->addAction(m_pActOpen);
    m_pOperatorToolBar->addAction(m_pActFind);
    m_pOperatorToolBar->addSeparator();
    m_pOperatorToolBar->addAction(m_pActPlay);
    m_pOperatorToolBar->addSeparator();
    m_pOperatorToolBar->addAction(m_pActFullScreen);

    m_pLbTimeAxis = new QLabel(this);                   // 时间轴组件  
    m_pLbTimeAxis->setPixmap(QPixmap(IMG_TIMELINE));
    m_pLbTimeAxis->setToolTip(trMenuString(cstDictTimelineModule));
    m_pLbTimeAxis->setProperty("ItemType", Item_TimeAxis);

    m_pLbVideo = new QLabel(this);                  // 视频组件  
    m_pLbVideo->setPixmap(QPixmap(IMG_VIDEO));
    m_pLbVideo->setToolTip(trMenuString(cstDictVideoModule));
    m_pLbVideo->setProperty("ItemType", Item_Video);

    m_pLbCurve = new QLabel(this);                  // 曲线图组件  
    m_pLbCurve->setPixmap(QPixmap(IMG_CURVE));
    m_pLbCurve->setToolTip(trMenuString(cstDictCurveModule));
    m_pLbCurve->setProperty("ItemType", Item_Chart);

    m_pLbTable = new QLabel(this);                  // 表格组件  
    m_pLbTable->setPixmap(QPixmap(IMG_TABLE));
    m_pLbTable->setToolTip(trMenuString(cstDictTableModule));
    m_pLbTable->setProperty("ItemType", Item_Table);

    m_pModuleToolBar = new CToolBar(this);
    this->addToolBar(m_pModuleToolBar);
    m_pModuleToolBar->addWidget(m_pLbTimeAxis);
    m_pModuleToolBar->addWidget(m_pLbVideo);
    m_pModuleToolBar->addWidget(m_pLbCurve);
    m_pModuleToolBar->addWidget(m_pLbTable);

    // 创建状态栏 
    this->setStatusBar(new QStatusBar(this));
}


void DAS::OnOpen()
{

}


void DAS::OnPlay()
{

}


void DAS::OnFind()
{

}


void DAS::OnFullScreen()
{

}


void DAS::OnLanuageChanged(QAction* pAction)
{
    if (pAction == m_pActZhCn)              // 简体中文 
    {
        m_pActEn->setChecked(false);
        m_pActZhCn->setChecked(true);
        if (m_translator.load(":/dict/das_zh_cn.qm"))
        {
            QApplication::installTranslator(&m_translator);
        }
    }
    else if (pAction == m_pActEn)           // 英语 
    {
        m_pActEn->setChecked(true);
        m_pActZhCn->setChecked(false);
        if (m_translator.load(":/dict/das_en.qm"))
        {
            QApplication::installTranslator(&m_translator);
        }
    }
}


void DAS::OnAbout()
{

}


void DAS::retranslate()
{
    m_pActOpen->setText(trMenuString(cstDictOpenPath));
    m_pActExit->setText(trMenuString(cstDictExit));
    m_pMenuFile->setTitle(trMenuString(cstFile));
    m_pActPlay->setText(trMenuString(cstDictPlay));    // 播放、暂停、停止，三相开关 

    m_pActFind->setText(trMenuString(cstDictFind));

    m_pMenuEdit->setTitle(trMenuString(cstEdit));
    m_pActFullScreen->setText(trMenuString(cstDictFullScreen));
    m_pMenuView->setTitle(trMenuString(cstView));
    //m_pActZhCn->setText(trMenuString(cstDictZhCn));
    //m_pActEn->setText(trMenuString(cstDictEn));

    m_pMenuTool->setTitle(trMenuString(cstTool));
    m_pMenuLanuage->setTitle(trMenuString(cstDictLanuage));

    m_pActAbout->setText(trMenuString(cstDictAbout));
    m_pMenuHelp->setTitle(trMenuString(cstHelp));

    m_pOperatorToolBar->setWindowTitle(trMenuString(cstOperatorToolBar));
    m_pLbTimeAxis->setToolTip(trMenuString(cstDictTimelineModule));
    m_pLbVideo->setToolTip(trMenuString(cstDictVideoModule));
    m_pLbCurve->setToolTip(trMenuString(cstDictCurveModule));
    m_pLbTable->setToolTip(trMenuString(cstDictTableModule));
    m_pModuleToolBar->setWindowTitle(trMenuString(cstModuleToolBar));
}


void DAS::changeEvent(QEvent *ev)
{
    int iType = ev->type();
    switch (iType)
    {
    case QEvent::LanguageChange:
        retranslate();
        break;
    default:
        break;
    }

    QMainWindow::changeEvent(ev);
}

