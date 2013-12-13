#include "vibeswindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    bool showFileOpenDlg = false;
    for (int i = 1; i < argc; ++i)
        if (!qstrcmp(argv[i], "--show-open-dlg"))
            showFileOpenDlg = true;

    VibesWindow w(showFileOpenDlg);
    w.show();

    return a.exec();
}
