#include "vibeswindow.h"
#include "ui_vibeswindow.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "figure2d.h"

#include <QGraphicsRectItem>
#include <QFileDialog>

#include <vibestreemodel.h>

VibesWindow::VibesWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VibesWindow)
{
    ui->setupUi(this);

    ui->treeView->setModel(new VibesTreeModel(figures, this));

    readFile();
}

VibesWindow::~VibesWindow()
{
    delete ui;
}

Figure2D * VibesWindow::newFigure(QString name)
{
    // Create a unique name if none has been provided
    if (name.isEmpty()) {
        name = "Figure";
        for (int i=2; figures.contains(name); ++i) {
            name = QString("Figure %1").arg(i);
        }
    }
    // Delete exixsting figure with the same name
    delete figures[name];
    // Create new figure
    figures[name] = new Figure2D(this);
    // Set flags to make it a window
    figures[name]->setWindowFlags(Qt::Window);
    figures[name]->setWindowTitle(name);
    figures[name]->show();
    // Return pointer to the new figure
    return figures[name];
}

bool VibesWindow::processMessage(const QByteArray &msg_data)
{
    QJsonParseError parse_error;
    QJsonDocument doc = QJsonDocument::fromJson(msg_data, &parse_error);
    // Check parsing error
    if (parse_error.error != QJsonParseError::NoError) {
        /// \todo Notify or error in input data
        return false;
    }
    // Check if our document is an object
    if (!doc.isObject()) {
        return false;
    }

    // Process message
    QJsonObject msg = doc.object();
    // Action is a mandatory field
    if (!msg.contains("action")) {
        return false;
    }

    QString action = msg["action"].toString();
    QString fig_name = msg["figure"].toString();

    // Get pointer to target figure
    Figure2D * fig = 0;
    if (figures.contains(fig_name))
        fig = figures[fig_name];

    // Close a figure
    if (action == "close")
    {
        if (fig) {
            figures.remove(fig_name);
            delete fig;
            return true;
        }
        else {
            return false;
        }
    }

    // Create a new figure
    else if (action == "new")
    {
        // Create a new figure (previous with the same name will be destroyed)
        fig = newFigure(fig_name);
    }

    // Draw a shape
    else if (action == "draw")
    {
        if (!fig) // Create a new figure if it does not exist
            fig = newFigure(fig_name);

        QColor fill_color;
        QColor edge_color;

        QGraphicsItem * item = 0;
        // Draw a box
        if (msg.contains("box"))
        {
            QJsonObject box = msg.value("box").toObject();
            QJsonArray bounds = box["bounds"].toArray();
            if (bounds.size()>=4) {
                double lb_x = bounds[0].toDouble();
                double ub_x = bounds[1].toDouble();
                double lb_y = bounds[2].toDouble();
                double ub_y = bounds[3].toDouble();

                item = fig->scene()->addRect(lb_x, lb_y, ub_x-lb_x, ub_y-lb_y);
            }
        }
    }

    // Unknown action
    else
    {
        return false;
    }
    return true;
}

void VibesWindow::readFile()
{
    /// \todo Put platform dependent code here for named pipe creation and opening
    QFile file(QFileDialog::getOpenFileName());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QByteArray message;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        // No data to read
        if (line.isEmpty()) {
            continue;
        }
        // Empty new line ("\n\n") is message separator
        else if (line[0]=='\n') {
            processMessage(message);
            message.clear();
        }
        // Add this line to the current message
        else {
            message.append(line);
        }
    }
}
