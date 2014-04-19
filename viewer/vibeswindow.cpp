#include "vibeswindow.h"
#include "ui_vibeswindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "figure2d.h"

#include "vibesscene2d.h"

#include <QFileDialog>

#include <QTimer>
#include <QtCore>

#include "vibestreemodel.h"

VibesWindow::VibesWindow(bool showFileOpenDlg, QWidget *parent) :
QMainWindow(parent),
ui(new Ui::VibesWindow)
{
    ui->setupUi(this);
    ui->treeView->setModel(new VibesTreeModel(figures, this));

    // When its name is double clicked in the list, the corresponding figure is brought to front
    connect(ui->treeView, &QTreeView::doubleClicked,
            [](const QModelIndex& mi){Figure2D* fig = static_cast<Figure2D*>( mi.internalPointer() );
                                      if (fig) { fig->showNormal();
                                                 fig->activateWindow();
                                                 fig->raise(); }
                                     } );

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

    // Delete existing figure with the same name
    if (figures.contains(name))
    {
        // Unname the figure that will be deleted, so that it is not backlinked to the list of figures
        figures[name]->setObjectName(QString());
        delete figures[name];
    }

    // Create new figure
    Figure2D * fig = new Figure2D(this);
    fig->setObjectName(name);

    // Update figure list
    figures[name] = fig;
    static_cast<VibesTreeModel*> (ui->treeView->model())->forceUpdate();
    this->connect(fig, SIGNAL(destroyed(QObject*)), SLOT(removeFigureFromList(QObject*)));

    // Set flags to make it a window
    fig->setWindowFlags(Qt::Window);
    fig->setWindowTitle(name);
    fig->show();
    fig->activateWindow();
    fig->raise();
    // Return pointer to the new figure
    return fig;
}

void VibesWindow::removeFigureFromList(QObject* fig)
{
    QHash<QString,Figure2D*>::iterator it = figures.find(fig->objectName());
    if (it != figures.end() && it.value()==fig)
    {
        figures.erase(it);
        static_cast<VibesTreeModel*> (ui->treeView->model())->forceUpdate();
    }
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
            fig->setSceneRect(lb_x, lb_y, ub_x - lb_x, ub_y - lb_y);
            fig->fitInView(fig->sceneRect());
        }
            // Auto-set the view rectangle
        else if (msg["box"].toString() == "auto")
        {
            fig->setSceneRect(QRectF());
            fig->fitInView(fig->sceneRect());
        }
    }
        // Export to a graphical file
    else if (action == "export")
    {
        // Figure has to exist
        if (!fig)
            return false;
        // Exports to given filename (if not defined, shows a save dialog)
        fig->exportGraphics(msg["file"].toString());
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
            // Let the scene parse JSON to create the appropriate object
            fig->scene()->addJsonShapeItem(shape);
        }
    }
        // Unknown action
    else
    {
        return false;
    }
    return true;
}

void VibesWindow::exportCurrentFigureGraphics()
{
    // Get current selected item in tree view
    QModelIndex selectId = ui->treeView->currentIndex();
    // If no selection, return
    if (!selectId.isValid())
        return;
    // If the selected item is a figure, export it
    Figure2D * pfig = static_cast<Figure2D*> (selectId.internalPointer());
    if (figures.values().contains(pfig))
    {
        pfig->exportGraphics();
    }
}

void
VibesWindow::readFile()
{
    // Display we are reading data
    if (!file.atEnd())
        ui->statusBar->showMessage("Receiving data...", 200);

    // Read and process data
    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        // No data to read
        if (line.isEmpty())
        {
            continue;
        }
            // Empty new line ("\n\n") is message separator
        else if (line[0] == '\n' && message.endsWith('\n'))
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

    // Program new file-read try in 50 ms.
    QTimer::singleShot(50, this, SLOT(readFile()));
}
