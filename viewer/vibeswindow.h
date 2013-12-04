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
    explicit VibesWindow(QWidget *parent = 0);
    ~VibesWindow();

    Figure2D * newFigure(QString name=QString());

public slots:
    void readFile();
    bool processMessage(const QByteArray &msg);

private:
    Ui::VibesWindow *ui;

    QHash<QString, Figure2D*> figures;
    QHash<QString, QBrush> brushes;

    QFile file;
    QPen defaultPen;

    void initDefaultBrushes();
};

#endif // VIBESWINDOW_H
