#ifndef VIBESWINDOW_H
#define VIBESWINDOW_H

#include <QMainWindow>

#include <QHash>
class Figure2D;

namespace Ui {
class VibesWindow;
}

class VibesWindow : public QMainWindow
{
    Q_OBJECT

    QHash<QString, Figure2D*> figures;

public:
    explicit VibesWindow(QWidget *parent = 0);
    ~VibesWindow();

    Figure2D * newFigure(QString name=QString());
    bool processMessage(const QByteArray &msg);
    void readFile();

private:
    Ui::VibesWindow *ui;
};

#endif // VIBESWINDOW_H
