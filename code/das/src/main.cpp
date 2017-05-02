#include "QtWidgets/QApplication"
#include "das.h"

#include "CLogManager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 加载多国语文件 
    QTranslator translator;
    if (translator.load(":/dict/das_zh_cn.qm"))
    {
        a.installTranslator(&translator);
    }

    DAS das;
    das.show();
    CLogManager::getInstance()->setWriteable(true);
    CLogManager::getInstance()->startLog("./");
    CLogManager::getInstance()->log(eLogDebug, "main", "start app!");

    // test interface 
    /*CCanDataProfile* pCan = new CCanDataProfile;
    int iRet = pCan->Init();
    int iCount = pCan->GetItemCount();
    CCanItemData* pData = pCan->GetCanItem(1);*/

    return a.exec();
}
