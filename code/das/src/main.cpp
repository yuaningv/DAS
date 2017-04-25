#include "QtWidgets/QApplication"
#include "das.h"

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

    return a.exec();
}
