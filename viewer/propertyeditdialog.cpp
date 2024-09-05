#include "propertyeditdialog.h"
#include "ui_propertyeditdialog.h"

#include <QDebug>

// TO DELETE
#include <iostream>

PropertyEditDialog::PropertyEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PropertyEditDialog)
{
    ui->setupUi(this);
}

PropertyEditDialog::~PropertyEditDialog()
{
    delete ui;
}

QJsonObject PropertyEditDialog::showEditorForJson(QJsonObject init)
{
    PropertyEditDialog *dlg = new PropertyEditDialog();
    dlg->ui->comboBox_LineStyle->setCurrentText(init["LineStyle"].toString());
    dlg->ui->comboBox_FaceColor->setCurrentText(init["FaceColor"].toString());
    dlg->ui->comboBox_EdgeColor->setCurrentText(init["EdgeColor"].toString());
    dlg->ui->lineEdit_LineWidth->setText(init["LineWidth"].toString());

    QJsonObject json;

    if (dlg->exec() == Accepted)
    {
        json["LineStyle"] = QJsonValue(dlg->ui->comboBox_LineStyle->currentText());
        json["FaceColor"] = QJsonValue(dlg->ui->comboBox_FaceColor->currentText());
        json["EdgeColor"] = QJsonValue(dlg->ui->comboBox_EdgeColor->currentText());
        json["LineWidth"] = QJsonValue(dlg->ui->lineEdit_LineWidth->text());
    }

    // qDebug() << "dlg json result" << json;

    return json;
}
