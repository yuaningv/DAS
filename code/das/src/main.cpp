#include "das.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DAS das;
    das.show();
  
    return a.exec();
}
