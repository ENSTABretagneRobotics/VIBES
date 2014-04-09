#ifndef VIBESWINDOW_H
#define VIBESWINDOW_H

#include <QMainWindow>

#include <QHash>
#include <QFile>
#include <QPen>
#include <QBrush>

class Figure2D;

namespace Ui {
class VibesWindow;
}

class VibesWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit VibesWindow(bool showFileOpenDlg=false, QWidget *parent = 0);
    ~VibesWindow();

    Figure2D * newFigure(QString name=QString());

public slots:
    void readFile();
    bool processMessage(const QByteArray &msg);
    void exportCurrentFigureGraphics();

private slots:
    void removeFigureFromList(QObject *fig);

private:
    Ui::VibesWindow *ui;

    QHash<QString, Figure2D*> figures;

    QFile file;
    QByteArray message;
};

#endif // VIBESWINDOW_H
