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


/// Add a graphics item to the scene from its JSON "shape" object description
/// \param[in] shape The JSON object containing properties
/// \returns The item created and initialized from JSON objet. Null pointer if creation or initialization failed.

VibesGraphicsItem * VibesScene2D::addJsonShapeItem(const QJsonObject &shape)
{
    // The graphics item that will be created (will be null if creation fails)
    VibesGraphicsItem * item = 0;

    if (shape.contains("type"))
    {
        QString type = shape["type"].toString();
        item = VibesGraphicsItem::newWithType(type);
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
}

bool VibesScene2D::setDimX(int dimX)
{
    if (dimX>=0 && dimX<nbDim() && dimX!=this->dimX() && dimX!=dimY())
    {
        _dimX = dimX;
        emit changedDimX(dimX);
        updateDims();
        return true;
    } else {
        // Notify that invalid change has been refused
        if (dimX != this->dimX()) emit changedDimX(this->dimX());
        return false;
    }
}

bool VibesScene2D::setDimY(int dimY)
{
    if (dimY>=0 && dimY<nbDim() && dimY!=this->dimY() && dimY!=dimX())
    {
        _dimY = dimY;
        emit changedDimY(dimY);
        updateDims();
        return true;
    } else {
        // Notify that invalid change has been refused
        if (dimY != this->dimY()) emit changedDimY(this->dimY());
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
        updateDims();
        return true;
    } else {
        // Notify that invalid change has been refused
        if (dimX != this->dimX()) emit changedDimX(this->dimX());
        if (dimY != this->dimY()) emit changedDimY(this->dimY());
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
