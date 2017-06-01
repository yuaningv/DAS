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
#include "StreamMgr.h"

class QLabel;


class CVideoWidget : public CCustomWidgetBase, public CStreamListener
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

    void setChannel(int iChannel);       // 通过channel来判断该组件是否已经关联数据，iChannel < 0无关联数据，iChannel >= 0关联数据 
    int getChannel() { return m_iChannel; };

protected:
    virtual void mouseReleaseEvent(QMouseEvent *ev);
    virtual void mouseDoubleClickEvent(QMouseEvent *ev);
    virtual void keyReleaseEvent(QKeyEvent *ev);

    virtual ITEMTYPE type();
    virtual void paintEvent(QPaintEvent *ev);

    virtual HWND GetWndHandle();
    virtual void OnMedia(unsigned char* buffer, unsigned long length, unsigned long payload, 
		CCustomDateTime* pTime, void* pCustomData);

private:
    int m_iID;
    QLabel* m_labelTitle;
    QString m_strTitle;

    ItemAttribute_t m_tItemAttr;
    bool m_bFullscreenFlag;
    bool m_bFlag;               // test video 

    int m_iChannel;
    QImage m_image;
    QMutex m_mutex;
private:
    int m_iLastX;
    int m_iLastY;
    int m_iLastWidth;
    int m_iLastHeight;
    CGraphicsView* m_pView;             // 获取View窗口大小 

private slots:
};


#endif // CVideoWidget_h

