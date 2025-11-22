#include "vibeswindow.h"
#include "ui_vibeswindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include "figure2d.h"

#include "vibesscene2d.h"
#include "vibesgraphicsitem.h"
#include "treeview.h"

#include <QFileDialog>

#include <QTimer>
#include <QtCore>

#include "vibestreemodel.h"

#include "propertyeditdialog.h"
#include <QJsonObject>

VibesWindow::VibesWindow(bool showFileOpenDlg, QWidget *parent) :
QMainWindow(parent),
ui(new Ui::VibesWindow),
bRemoveFileOnExit(false)
{
    ui->setupUi(this);
    ui->treeView->setModel(new VibesTreeModel(figures, this));

    // When its name is double clicked in the list, the corresponding figure is brought to front
    /*connect(ui->treeView, &QTreeView::doubleClicked,
            [this](const QModelIndex& mi){//Figure2D* fig = static_cast<Figure2D*>( mi.internalPointer() );
                                      Figure2D* fig = 0;
                                      if (mi.internalPointer() == 0) { fig = this->figures.values().at(mi.row()); }
                                      if (fig) { fig->showNormal();
                                                 fig->activateWindow();
                                                 fig->raise(); }
                                     } );
                                     */
    // When a object name is double clicked in the list, the property editor is shown
    /*connect(ui->treeView, &QTreeView::doubleClicked,
            [](const QModelIndex& mi){Figure2D* fig = static_cast<Figure2D*>( mi.internalPointer() );
                                      if (fig) { VibesScene2D * scene = fig->scene();
                                                 VibesGraphicsItem * item = scene->itemByName(scene->namedItems().at(mi.row()));
                                                 QJsonObject json = PropertyEditDialog::showEditorForJson(item->json());
                                                 item->setJsonValues(json);
                                               }
                                     } );
    */
    // When DEL is pressed, remove the corresponding figure
    connect(ui->treeView, SIGNAL(deleteFigureEvent()),this,SLOT(closeSingleGraphic()));
    // When H is pressed, hide the corresponding figure
    connect(ui->treeView, SIGNAL(hideFigureEvent()),this,SLOT(hideSingleGraphic()));
    // When S is pressed or when its name is double left clicked in the list, show the corresponding figure
    connect(ui->treeView, SIGNAL(showFigureEvent()),this,SLOT(showSingleGraphic()));
    // When P is pressed, the property editor is shown
    connect(ui->treeView, SIGNAL(propertiesEvent()),this,SLOT(editProperties()));
    /// \todo Put platform dependent code here for named pipe creation and opening
    if (showFileOpenDlg)
    {
        file.setFileName(QFileDialog::getOpenFileName());
        bRemoveFileOnExit = false;
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
        // Indicate the file should be removed on program exit
        bRemoveFileOnExit = true;
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

    // Remove .vibes.json file
    if (bRemoveFileOnExit)
    {
        file.remove();
    }
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
        // Clear the contents of a figure or a group
    else if (action == "clear")
    {
        // Figure has to exist
        if (!fig)
            return false;
        if (msg.contains("group"))
        {
            // Clears the group
            VibesGraphicsGroup *group = 0;
            group = vibesgraphicsitem_cast<VibesGraphicsGroup *>(fig->scene()->itemByName(msg["group"].toString()));
            if (!group)
                return false;
            group->clear();
        }
        else
        {
            // Clears the scene
            fig->scene()->clear();
        }
        /// \todo Remove named objects references if needed
    }
        // Deletes a graphics item
    else if (action == "delete")
    {
        // Figure has to exist
        if (!fig)
            return false;
        if (!msg.contains("object"))
            return false;
        // retrieve item by name
        VibesGraphicsItem * item = fig->scene()->itemByName(msg["object"].toString());
        // if named item exists...
        if (!item)
            return false;
        // ...delete it
        delete item;
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

        if (msg.contains("shape"))
        {
            QJsonObject shape = msg.value("shape").toObject();
            // Let the scene parse JSON to create the appropriate object
            fig->scene()->addJsonShapeItem(shape);
        }
    }
        // Set properties
    else if (action == "set")
    {
        // Figure has to exist
        if (!fig)
            return false;
        // Set object properties
        if (msg.contains("object"))
        {
            // Find named object
            VibesGraphicsItem * object = fig->scene()->itemByName(msg.value("object").toString());
            if (!object)
                return false;
            // Update properties
            QJsonObject properties = msg.value("properties").toObject();
            for (QJsonObject::const_iterator it = properties.constBegin(); it != properties.constEnd(); it++)
            {
                object->setJsonValue(it.key(), it.value());
            }
        }
        // else set figure properties
        else
        {
            // Update properties
            QJsonObject properties = msg.value("properties").toObject();
            for (QJsonObject::const_iterator it = properties.constBegin(); it != properties.constEnd(); it++)
            {
                if (it.key() == "width" && it.value().toDouble()>40.0)
                {
                    fig->resize( it.value().toDouble(), fig->height() );
                }
                else if (it.key() == "height" && it.value().toDouble()>40.0)
                {
                    fig->resize( fig->width(), it.value().toDouble() );
                }
                else if (it.key() == "x")
                {
                    fig->move( it.value().toDouble(), fig->y() );
                }
                else if (it.key() == "y")
                {
                    fig->move( fig->x(), it.value().toDouble() );
                }
                else if (it.key() == "viewbox")
                {
                    // Set the view rectangle to a box
                    if (it.value().isArray())
                    {
                        const QJsonArray box = it.value().toArray();
                        if (box.size() < 4) return false;
                        double lb_x = box[0].toDouble();
                        double ub_x = box[1].toDouble();
                        double lb_y = box[2].toDouble();
                        double ub_y = box[3].toDouble();
                        fig->setSceneRect(lb_x, lb_y, ub_x - lb_x, ub_y - lb_y);
                        fig->fitInView(fig->sceneRect());
                    }
                    // Auto-set the view rectangle
                    else if (it.value().toString() == "auto")
                    {
                        fig->setSceneRect(QRectF());
                        fig->fitInView(fig->sceneRect());
                    }
                    // Auto-set the view rectangle with equal side size
                    else if (it.value().toString() == "equal")
                    {
                        fig->setSceneRect(QRectF());
                        QRectF sceneSize=fig->sceneRect();
                        qreal x, y, w, h;
                        sceneSize.getRect(&x,&y,&w,&h);

                        //>[#144]
                        qreal u=x+w/2.0, v=y+h/2.0;
                        qreal m=static_cast<qreal>(fig->width());
                        qreal n=static_cast<qreal>(fig->height());
                        qreal cw = h/n*m;
                        qreal ch = w/m*n;
                        if(cw>w){ w=cw; x=u-w/2.0; }
                        else    { h=ch; y=v-h/2.0; }
                        fig->setSceneRect(x,y,w,h);
                        //<[#144]

                        fig->fitInView(fig->sceneRect());
                    }

                }
                else if (it.key() == "axisdims"){ //[#148]
                    bool changedDimX=false, changedDimY=false;
                    const QJsonArray dims = it.value().toArray();
                    if(0<dims.size()){
                        changedDimX=fig->scene()->setDimX(dims[0].toInt());
                    }
                    if(1<dims.size()){
                        changedDimY=fig->scene()->setDimY(dims[1].toInt());
                    }
                    /* first x-axis modification may fails if requested
                     * dimension is in use in y-axis, second modification
                     * in this case */
                    if(0<dims.size()){
                        changedDimX=fig->scene()->setDimX(dims[0].toInt());
                    }
                    if((!changedDimX)||(!changedDimY)){
                        return false;
                    }
                }
                else if (it.key() == "axislabels")
                {
                    const QJsonArray labels = it.value().toArray();
                    for (int i=0; i<labels.size(); i++)
                        fig->scene()->setDimName(i, labels[i].toString());
                }
                else if (it.key() == "showAxis"){
                    bool value = it.value().toBool();
                    fig->setShowAxis(value);
                }


            }
        }
    }
        // Unknown action
    else
    {
        return false;
    }
    const QModelIndex index = ui->treeView->selectionModel()->currentIndex(); //Save the selection before updating 

    ui->treeView->setSelectionMode(QAbstractItemView::MultiSelection); // Save the list of expanded items
    ui->treeView->selectAll();
    const QModelIndexList indexList = ui->treeView->selectionModel()->selectedIndexes();
    ui->treeView->clearSelection();
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    QHash<QModelIndex,bool> isExpandedList;
    for(int i = 0; i < indexList.size();i++)
    {
        isExpandedList[indexList[i]] = ui->treeView->isExpanded(indexList[i]); 
    }

    /// \todo Do not force update after each message

    static_cast<VibesTreeModel*> (ui->treeView->model())->forceUpdate();


    ui->treeView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows); //Apply the saved selection
    for(int i = 0; i < indexList.size();i++)
    {
        ui->treeView->setExpanded(indexList[i],isExpandedList[indexList[i]]); // Apply the saved list of expanded items
    }

    return true;
}

void VibesWindow::editProperties()
{
    //Edit properties of a group
    const QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QString selectedFigure = index.parent().data(Qt::DisplayRole).toString();
    if(figures.find(selectedFigure) != figures.end()) //Check if the parent of the selection is a figure
    {
        VibesScene2D * scene = figures[selectedFigure]->scene();
        QString selectedGroup = index.data(Qt::DisplayRole).toString();
        if(scene->namedItems().indexOf(selectedGroup) != -1)
        {
            VibesGraphicsItem * item = scene->itemByName(selectedGroup);
            QJsonObject json = PropertyEditDialog::showEditorForJson(item->json());
            item->setJsonValues(json);
        }
        
    }

}

void VibesWindow::closeSingleGraphic()
{
    //Close selected figure
    //Find figure's name
    const QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QString selectedFigure = index.data(Qt::DisplayRole).toString();
    if(figures.find(selectedFigure) != figures.end()) //Check if a figure is selected (and not a group)
    {
        delete figures[selectedFigure];
        static_cast<VibesTreeModel*> (ui->treeView->model())->forceUpdate();
    }
}

void VibesWindow::hideSingleGraphic()
{
    //Hide selected figure
    //Find figure's name
    const QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QString selectedFigure = index.data(Qt::DisplayRole).toString();
    if(figures.find(selectedFigure) != figures.end()) //Check if a figure is selected (and not a group)
    {
        figures[selectedFigure]->hide();
    }
}

void VibesWindow::showSingleGraphic()
{
    //Show selected figure
    //Find figure's name
    const QModelIndex index = ui->treeView->selectionModel()->currentIndex();
    QString selectedFigure = index.data(Qt::DisplayRole).toString();
    if(figures.find(selectedFigure) != figures.end()) //Check if a figure is selected (and not a group)
    {
        figures[selectedFigure]->hide(); //In case the figure is reduced
        figures[selectedFigure]->show();
    }    
}

void VibesWindow::closeAllGraphics()
{
    //Close all graphics
    if(figures.size() != 0)
    {
        int answer = QMessageBox::question(this, "VIBes", tr("You are about to close all the figures in a definitive manner. Are you sure?"),QMessageBox::Yes|QMessageBox::No);
        if(answer == QMessageBox::Yes)
        {
            QHashIterator<QString, Figure2D*> i(figures);
            while (i.hasNext())
            {
                i.next();
                delete i.value();
            }
            static_cast<VibesTreeModel*> (ui->treeView->model())->forceUpdate();
        }
    }
}

void VibesWindow::openAllGraphics()
{
    //Open all graphics
    QHashIterator<QString, Figure2D*> i(figures);
    while (i.hasNext())
    {
        i.next();
        i.value()->hide(); //In case the figure is reduced
        i.value()->show();
    }
}

void VibesWindow::hideAllGraphics()
{
    //Hide all graphics
    QHashIterator<QString, Figure2D*> i(figures);
    while (i.hasNext())
    {
        i.next();
        i.value()->hide();
    }
}

void VibesWindow::exportCurrentFigureGraphics()
{
    // Get current selected item in tree view
    QModelIndex selectId = ui->treeView->currentIndex();
    // If no selection, return
    if (!selectId.isValid())
        return;
    // If the selected item is a figure, export it
    Figure2D * pfig = 0;
    if (!selectId.internalPointer())
        pfig = figures.values().at(selectId.row());
    else
        pfig = static_cast<Figure2D*> (selectId.internalPointer());

    if (figures.values().contains(pfig))
    {
        pfig->exportGraphics();
    }
}

void VibesWindow::openHelpDialog()
{
    QMessageBox::information(this, "VIBes", tr("Open a figure: O or double-click\n"
                                               "Hide a figure: H\n"
                                               "Close a figure: DEL\n"
                                               "Edit group properties: P or double-click\n"
                                               "Right-click on an item opens a menu with the same options\n"
                                               "Zoom in: + or Q\n"
                                               "Zoom out: - or W\n"
                                               "Toggle axis view: A\n"
                                               "Change axis ticks: repeated press on X, S, Y, H\n"
                                               "Change font size: * or F and / or V\n"
                                               "Default view settings: SPACE"));
}

void VibesWindow::readFile()
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
