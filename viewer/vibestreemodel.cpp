#include "vibestreemodel.h"
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

    const void *parentItem;

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
    }

    return QModelIndex();
}

QModelIndex VibesTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    const void *childItem = index.internalPointer();
    if (childItem == figures)
    {
        return QModelIndex();
    }

    if (!figures->key(static_cast<Figure2D*>(index.internalPointer())).isEmpty())
    {
        return QModelIndex();
    }


    return QModelIndex();
}

int VibesTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    const void *parentItem;
    if (!parent.isValid())
        parentItem = figures;
    else
        parentItem = parent.internalPointer();

    if (parentItem == figures)
    {
        return figures->size();
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
        if (childItem == figures)
        {
            return QVariant();
        }
        if (const Figure2D* childFigure = static_cast<const Figure2D*>(childItem))
        {
            return QColor(Qt::red);
        }
        return QVariant();
    }

    if (role != Qt::DisplayRole)
        return QVariant();

    const void *childItem = index.internalPointer();
    if (childItem == figures)
    {
        return QVariant("Top level");
    }
    if (const Figure2D* childFigure = static_cast<const Figure2D*>(childItem))
    {
        return figures->key(const_cast<Figure2D*>(childFigure), "Unnamed figure");
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
