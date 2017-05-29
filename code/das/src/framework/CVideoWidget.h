/*
* File    : CVideoWidget.h
*
* Date    : 2017-04-25
*
* Author  : 
*
* Brief   : 
*
* Note    :
*/

#ifndef CVideoWidget_h
#define CVideoWidget_h


#include "CCustomWidgetBase.h"
#include "CGraphicsView.h"

#include "type.h"
#include "CVideoDataListener.h"

class QLabel;


class CVideoWidget : public CCustomWidgetBase
{
    Q_OBJECT

public:
    explicit CVideoWidget(QWidget* parent = 0);
    ~CVideoWidget();

public:
    void setID(int iID) { m_iID = iID; };
    int getID() { return m_iID; };
    void setTitle(const QString& strTitle);
    QString getTitle() { return m_strTitle; };

    void setItemAttr(ItemAttribute_t itemAttr);
    ItemAttribute_t getItemAttr();

    void setView(CGraphicsView* pView) { m_pView = pView; };

    void updateFrame();

public:
    HWND getWinId() { return (HWND)this->winId(); };

    int init(const QString& strStorage, int iChannel = -1);
    void setChannel(int iChannel);       // 通过channel来判断该组件是否已经关联数据，iChannel < 0无关联数据，iChannel >= 0关联数据 
    int getChannel() { return m_iChannel; };
    int setScape(const QDateTime& dtBegin, const QDateTime& dtEnd);
    int skipTo(const QDateTime& dtPos);
    int play();
    int pause();

protected:
    virtual void mouseReleaseEvent(QMouseEvent *ev);
    virtual void mouseDoubleClickEvent(QMouseEvent *ev);
    virtual void keyReleaseEvent(QKeyEvent *ev);

    virtual ITEMTYPE type();
    virtual void paintEvent(QPaintEvent *ev);

private:
    int m_iID;
    QLabel* m_labelTitle;
    QString m_strTitle;

    ItemAttribute_t m_tItemAttr;
    bool m_bFullscreenFlag;
    bool m_bFlag;           // test video 

    // 关联video数据 
    CVideoDataListener* m_pVideoData;
    int m_iChannel;
    QString m_strStorage;
    QImage m_image;

private:
    int m_iLastX;
    int m_iLastY;
    int m_iLastWidth;
    int m_iLastHeight;
    CGraphicsView* m_pView;             // 获取View窗口大小 

private slots:
    void OnUpdateFrame(const QImage& image, const QDateTime& currentDateTime);
};


#endif // CVideoWidget_h

