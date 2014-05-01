#include "vibesscene2d.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QtCore>

#include "vibesgraphicsitem.h"

/// Default Constructor

VibesScene2D::VibesScene2D(QObject *parent) :
    QGraphicsScene(parent),
    _dimX(0), _dimY(1), _nbDim(2)
{
}

/// Destructor

VibesScene2D::~VibesScene2D()
{
    // All objects are removed from the scene in VibesScene2D destructor.
    // This is necessary in ordrer to avoid BAD_ACCESS, since named objects call
    // setItemName of the scene to unregister their name when they die. Otherwise,
    // since VibesScene2D is destructed before QGraphicsScene, the hash _namedItems
    // would not exist anymore when QGraphicsScene destructor would destroy the items

    foreach (QGraphicsItem* item, this->items())
        delete item;
}



/// Add a graphics item to the scene from its JSON "shape" object description
/// \param[in] shape The JSON object containing properties
/// \returns The item created and initialized from JSON objet. Null pointer if creation or initialization failed.

VibesGraphicsItem * VibesScene2D::addJsonShapeItem(const QJsonObject &shape)
{
    // The graphics item that will be created (will be null if creation fails)
    VibesGraphicsItem * item = 0;
    // The group to which the item will belong (will be null if no group specified, or if group does not exist)
    VibesGraphicsGroup * group = 0;

    // Contruct a new object from given type string
    if (shape.contains("type"))
    {
        QString type = shape["type"].toString();
        item = VibesGraphicsItem::newWithType(type);
    }

    // Find the object parent group if specified
    if (shape.contains("group"))
    {
        QString groupName = shape["group"].toString();
        group = vibesgraphicsitem_cast<VibesGraphicsGroup*>( itemByName(groupName) );
    }

    // Try to initialize item with JSON
    if (!item->setJson(shape, dimX(), dimY()))
    {
        // Cannot set item wth the provided Json, delete item
        delete item;
        item = 0;
    }
/*    else
    {
        // Item successufully initialized, update dimension
        _nbDim = qMax(_nbDim, vibesItem->dimension());
    }
*/
    // If the item has successfully been created and initialized, put it on the scene
    if (item)
    {
        this->addVibesItem(item);
        // If the item belongs to a group, add it to the group
        if (group)
        {
            group->addToGroup(item);
        }
    }

    return item;
}

void VibesScene2D::addVibesItem(VibesGraphicsItem *item)
{
    if (!item) return;
    // Update scene dimension
    _nbDim = qMax(_nbDim, item->dimension());
    // Add item to the scene
    this->addItem(vibesgraphicsitem_cast<QGraphicsItem*>(item));
    // Update list of named objects
    this->setItemName(item, item->name());
}

void VibesScene2D::setItemName(VibesGraphicsItem *item, QString name)
{
    // Unname previous item with the same name
    if (VibesGraphicsItem *old_item = itemByName(name))
    {
        // Nothing to do if the name already points to the good item
        if (old_item != item)
            return;
        // Unname previous item
        old_item->setName(QString());
    }
    // Update named items list
    if (!name.isEmpty())
    {
        _namedItems[name] = item;
    }
    // Update new item if needed
    if (item && item->name() != name)
    {
        item->setName(name);
    }
}

bool VibesScene2D::setDimX(int dimX)
{
    if (dimX>=0 && dimX<nbDim() && dimX!=this->dimX() && dimX!=dimY())
    {
        _dimX = dimX;
        emit changedDimX(dimX);
        emit dimensionsChanged();
        updateDims();
        return true;
    } else {
        // Notify that invalid change has been refused
        if (dimX != this->dimX()) { emit changedDimX(this->dimX()); emit dimensionsChanged(); }
        return false;
    }
}

bool VibesScene2D::setDimY(int dimY)
{
    if (dimY>=0 && dimY<nbDim() && dimY!=this->dimY() && dimY!=dimX())
    {
        _dimY = dimY;
        emit changedDimY(dimY);
        emit dimensionsChanged();
        updateDims();
        return true;
    } else {
        // Notify that invalid change has been refused
        if (dimY != this->dimY()) { emit changedDimY(this->dimY()); emit dimensionsChanged(); }
        return false;
    }
}

bool VibesScene2D::setDims(int dimX, int dimY)
{
    if (dimX == this->dimX())
        return setDimY(dimY);
    else if (dimY == this->dimY())
        return setDimX(dimX);
    else if (dimY>=0 && dimY<nbDim() && dimX>=0 && dimX<nbDim() && dimX!=dimY)
    {
        _dimX = dimX;
        _dimY = dimY;
        emit changedDimX(dimX);
        emit changedDimY(dimY);
        emit dimensionsChanged();
        updateDims();
        return true;
    } else {
        // Notify that invalid change has been refused
        if (dimX != this->dimX()) { emit changedDimX(this->dimX()); emit dimensionsChanged(); }
        if (dimY != this->dimY()) { emit changedDimY(this->dimY()); emit dimensionsChanged(); }
        return false;
    }
}

void VibesScene2D::updateDims()
{
    QList<QGraphicsItem*> children = this->items();
    foreach(QGraphicsItem *item, children)
    {
        if (VibesGraphicsItem * vibesItem = qgraphicsitem_cast<VibesGraphicsItem*>(item))
        {
            if ( vibesItem->setProj(dimX(),dimY()) )
                item->setVisible(true);
            else
                item->setVisible(false);
        }
    }
    setSceneRect(itemsBoundingRect());
}
