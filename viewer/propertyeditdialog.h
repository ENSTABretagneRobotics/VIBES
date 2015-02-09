#ifndef PROPERTYEDITDIALOG_H
#define PROPERTYEDITDIALOG_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class PropertyEditDialog;
}

class PropertyEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PropertyEditDialog(QWidget *parent = 0);
    ~PropertyEditDialog();

private:
    Ui::PropertyEditDialog *ui;

public:
    static QJsonObject showEditorForJson(QJsonObject init);
};

#endif // PROPERTYEDITDIALOG_H
