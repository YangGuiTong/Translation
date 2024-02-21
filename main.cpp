#include "translation.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Translation w;
    w.show();

    return a.exec();
}
