#include "vibeswindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VibesWindow w;
    w.show();

    return a.exec();
}
