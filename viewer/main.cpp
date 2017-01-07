#include "vibeswindow.h"
#include <QApplication>
#include <QLocalSocket>
#include <QLocalServer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Identifier for running instance checking
    const QString serverName("VIBes_running_instance");
    // Check if another instance is already running
    {
        QLocalSocket socket;
        socket.connectToServer(serverName);
        if (socket.waitForConnected(500))
            return 1; // Exit already a process running
    }
    // Start a local server to signal the application is running
    // parent is set to <a> in order to destroy cleanly the socket at the end.
    QLocalServer * m_localServer = new QLocalServer(&a);
    m_localServer->listen(serverName);

    // Process command line arguments
    bool showFileOpenDlg = a.arguments().contains("--show-open-dlg", Qt::CaseInsensitive);

    // Create application main window
    VibesWindow w(showFileOpenDlg);
    w.show();

    // Trying to launch the app when it is already launched will make it pop
    QObject::connect(m_localServer, SIGNAL(newConnection()), &w, SLOT(showNormal()));
    QObject::connect(m_localServer, &QLocalServer::newConnection, &w, &VibesWindow::activateWindow);

    // Enter main event loop
    return a.exec();
}
