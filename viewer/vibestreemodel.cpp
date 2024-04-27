#include "vibestreemodel.h"
#include "vibesgraphicsitem.h"
#include "figure2d.h"
#include <QGraphicsItem>

VibesTreeModel::VibesTreeModel(const QHash<QString, Figure2D *> &figures, QObject *parent) :
    QAbstractItemModel(parent), figures(&figures)
{
}

QModelIndex VibesTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

 /*   const void *parentItem;

    if (!parent.isValid())
        parentItem = figures;
    else
        parentItem = parent.internalPointer();

    if (!parentItem)
        return QModelIndex();

    if (parentItem == figures)
    {
        if (row < figures->size())
            return createIndex(row, column, figures->values().at(row));
        else
            return QModelIndex();
    }*/

    if (!parent.isValid())
    {
        if (row < figures->size())
            return createIndex(row, column, (void*)0);
        else
            return QModelIndex();
    }

    if (parent.internalPointer() == 0)
    {
        return createIndex(row, column, figures->values().at(parent.row()));
    }



    return QModelIndex();
}

QModelIndex VibesTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    // Parent of a figure is root
    // figures have 0 as internal ptr
    const void *childItem = index.internalPointer();
    if (childItem == 0)
    {
        return QModelIndex();
    }

/*    if (!figures->key(static_cast<Figure2D*>(index.internalPointer())).isEmpty())
    {
        return QModelIndex(0, 0, 0);
    }
    */
    QList<Figure2D*> figPtrs = figures->values();
    int row = figPtrs.indexOf(static_cast<Figure2D*>(index.internalPointer()));
    if (row >= 0)
    {
        return createIndex(row, 0, (void*)0);
    }

    return QModelIndex();
}

int VibesTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;
/*
    const void *parentItem;
    if (!parent.isValid())
        parentItem = figures;
    else
        parentItem = parent.internalPointer();

    if (parentItem == figures)
    {
        return figures->size();
    }
    */

    // root node
    if (!parent.isValid())
    {
        return figures->size();
    }
    // figure node
    if (parent.internalPointer() == 0)
    {
        Figure2D * fig = figures->values().at(parent.row());
        return fig->scene()->namedItems().size();
    }

    return 0;
}

int VibesTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 1;
    else
        return 1;
}

QVariant VibesTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DecorationRole)
    {
        const void *childItem = index.internalPointer();
        if (childItem == 0)
        {
            //return QColor(Qt::green);
        }
        if (const Figure2D* childFigure = static_cast<const Figure2D*>(childItem))
        {
            //If the group have a face color or a edge color, a square of that color will appear next to its name in the tree
            VibesGraphicsItem * item = childFigure->scene()->itemByName(childFigure->scene()->namedItems().at(index.row()));
            QString edgeColor = item->jsonValue("EdgeColor").toString();
            QString faceColor = item->jsonValue("FaceColor").toString();
            if(faceColor != "")
            {
                return QColor(faceColor);
            }
            else if(edgeColor != "")
            {
                return QColor(edgeColor);
            }
            
        }
        return QVariant();
    }

    if (role != Qt::DisplayRole)
        return QVariant();

    const void *childItem = index.internalPointer();
    if (childItem == 0)
    {
        //return QVariant("Top level");
        //return figures->key(const_cast<Figure2D*>(childFigure), "Unnamed figure");
        Figure2D* fig = figures->values().at(index.row());
        return figures->key(fig, "Unnamed figure");
    }
    if (const Figure2D* childFigure = static_cast<const Figure2D*>(childItem))
    {
        //return figures->key(const_cast<Figure2D*>(childFigure), "Unnamed figure");
        //return QString("id=%1 row=%2").arg(index.internalId()).arg(index.row());
        return childFigure->scene()->namedItems().at(index.row());
    }

    return QVariant();
}

QVariant VibesTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if (section == 0) return "Object name";
    }

    return QVariant();
}
