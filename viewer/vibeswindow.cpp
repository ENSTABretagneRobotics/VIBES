#include "vibeswindow.h"
#include "ui_vibeswindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "figure2d.h"

#include <QGraphicsRectItem>
#include <QFileDialog>

#include <QTimer>
#include <QtCore>

#include <vibestreemodel.h>

VibesWindow::VibesWindow(bool showFileOpenDlg, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VibesWindow),
    defaultPen(Qt::black, 0)
{
    ui->setupUi(this);
    ui->treeView->setModel(new VibesTreeModel(figures, this));

    // Init. brushes for default color names
    initDefaultBrushes();

    /// \todo Put platform dependent code here for named pipe creation and opening
    if (showFileOpenDlg)
    {
        file.setFileName(QFileDialog::getOpenFileName());
    }
    else
    {
        QString file_name = "vibes.json";

        // Retrieve user-profile directory from environment variable
        QByteArray user_dir = qgetenv("USERPROFILE"); // Windows
        if (user_dir.isNull())
            user_dir = qgetenv("HOME"); // POSIX
        if (!user_dir.isNull())
        { // Environment variable found, connect to a file in user's profile directory
            file_name = user_dir;
            file_name.append("/.vibes.json");
        }

        file.setFileName(file_name);
        // Create and erase file if needed
        if (file.open(QIODevice::WriteOnly))
        {
            file.close();
        }
    }

    // Try to open the shared file
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->statusBar->showMessage(QString("Unable to load file %1.").arg(file.fileName()), 2000);
    }
    else
    {
        ui->statusBar->showMessage(QString("Reading file %1.").arg(file.fileName()), 2000);
        readFile();
    }
}

VibesWindow::~VibesWindow()
{
    delete ui;
}

/// Initializes brushes for default color names
void VibesWindow::initDefaultBrushes()
{
#define ADD_DEFAULT_BRUSH(full_name) \
    brushes[ #full_name ]  = QBrush(Qt::full_name);

#define ADD_DEFAULT_BRUSH2(full_name, short_name) \
    brushes[ #full_name ]  = QBrush(Qt::full_name); \
    brushes[ #short_name ] = QBrush(Qt::full_name);

    // Default brush
    brushes[QString()] = QBrush();

    // Named brushes
    ADD_DEFAULT_BRUSH2(cyan,c);
    ADD_DEFAULT_BRUSH2(yellow,y);
    ADD_DEFAULT_BRUSH2(magenta,m);
    ADD_DEFAULT_BRUSH2(red,r);
    ADD_DEFAULT_BRUSH2(green,g);
    ADD_DEFAULT_BRUSH2(blue,b);
    ADD_DEFAULT_BRUSH2(black,k);
    ADD_DEFAULT_BRUSH(darkGray);
    ADD_DEFAULT_BRUSH(gray);
    ADD_DEFAULT_BRUSH(lightGray);
    ADD_DEFAULT_BRUSH(darkCyan);
    ADD_DEFAULT_BRUSH(darkYellow);
    ADD_DEFAULT_BRUSH(darkMagenta);
    ADD_DEFAULT_BRUSH(darkRed);
    ADD_DEFAULT_BRUSH(darkGreen);
    ADD_DEFAULT_BRUSH(darkBlue);
}

Figure2D *
VibesWindow::newFigure(QString name)
{
    // Create a unique name if none has been provided
    if (name.isEmpty())
    {
        name = "Figure";
        for (int i = 2; figures.contains(name); ++i)
        {
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
    figures[name]->activateWindow();
    figures[name]->raise();
    // Return pointer to the new figure
    return figures[name];
}

bool
VibesWindow::processMessage(const QByteArray &msg_data)
{
    QJsonParseError parse_error;
    QJsonDocument doc = QJsonDocument::fromJson(msg_data, &parse_error);
    // Check parsing error
    if (parse_error.error != QJsonParseError::NoError)
    {
        /// \todo Notify or error in input data
        return false;
    }
    // Check if our document is an object
    if (!doc.isObject())
    {
        return false;
    }

    // Process message
    QJsonObject msg = doc.object();
    // Action is a mandatory field
    if (!msg.contains("action"))
    {
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
        // Figure has to exist to be closed
        if (!fig)
            return false;
        // Remove from the list of figures an delete
        figures.remove(fig_name);
        delete fig;
    }
    // Create a new figure
    else if (action == "new")
    {
        // Create a new figure (previous with the same name will be destroyed)
        fig = newFigure(fig_name);
    }
    // Clear the contents of a figure
    else if (action == "clear")
    {
        // Figure has to exist
        if (!fig)
            return false;
        // Clears the scene
        fig->scene()->clear();
        /// \todo Remove named objects references if needed
    }
    // Sets the view
    else if (action == "view")
    {
        // Figure has to exist
        if (!fig)
            return false;
        // Set the view rectangle to a box
        if (msg["box"].isArray())
        {
            const QJsonArray box = msg["box"].toArray();
            if (box.size() < 4) return false;
            double lb_x = box[0].toDouble();
            double ub_x = box[1].toDouble();
            double lb_y = box[2].toDouble();
            double ub_y = box[3].toDouble();
            fig->fitInView(lb_x, lb_y, ub_x - lb_x, ub_y - lb_y);
        }
        // Auto-set the view rectangle
        else if (msg["box"].toString() == "auto")
        {
            fig->fitInView(fig->scene()->sceneRect());
        }
    }
    // Draw a shape
    else if (action == "draw")
    {
        if (!fig) // Create a new figure if it does not exist
            fig = newFigure(fig_name);

        QColor fill_color;
        QColor edge_color;

        QGraphicsItem * item = 0;

        if (msg.contains("shape"))
        {
            QJsonObject shape = msg.value("shape").toObject();
            // Get shape color (or default if not specified)
            const QBrush & brush = brushes[shape.value("color").toString()];
            if (shape.contains("type"))
            {
                QString type = shape["type"].toString();

                if (type == "box")
                {
                    QJsonArray bounds = shape["bounds"].toArray();
                    if (bounds.size() >= 4)
                    {
                        double lb_x = bounds[0].toDouble();
                        double ub_x = bounds[1].toDouble();
                        double lb_y = bounds[2].toDouble();
                        double ub_y = bounds[3].toDouble();

                        item = fig->scene()->addRect(lb_x, lb_y, ub_x - lb_x, ub_y - lb_y, defaultPen, brush);
                    }
                }
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

void
VibesWindow::readFile()
{
    // Display we are reading data
    if (!file.atEnd())
        ui->statusBar->showMessage("Receiving data...", 200);

    // Read and process data
    QByteArray message;
    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        // No data to read
        if (line.isEmpty())
        {
            continue;
        }
        // Empty new line ("\n\n") is message separator
        else if (line[0] == '\n')
        {
            processMessage(message);
            message.clear();
        }
        // Add this line to the current message
        else
        {
            message.append(line);
        }
    }

    // Program new file-read try in 200 ms.
    QTimer::singleShot(200, this, SLOT(readFile()));
}
