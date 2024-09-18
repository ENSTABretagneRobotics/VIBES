#include "vibesgraphicsitem.h"

#include <QtCore>
#include <QVector>

#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsItemGroup>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QBitmap>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QDebug>
#include <cmath>
using namespace std;


// The only instance of VibesDefaults
VibesDefaults VibesDefaults::_instance;

VibesDefaults::VibesDefaults()
{
    initDefaultBrushesAndPens();
}

/// Initializes brushes and pens for default color names

void VibesDefaults::initDefaultBrushesAndPens()
{
#define ADD_DEFAULT_BRUSH(full_name) \
    _brushes[ #full_name ]  = QBrush(Qt::full_name);

#define ADD_DEFAULT_BRUSH2(full_name, short_name) \
    _brushes[ #full_name ]  = QBrush(Qt::full_name); \
    _brushes[ #short_name ] = QBrush(Qt::full_name);

#define ADD_DEFAULT_PEN(full_name) \
    _pens[ #full_name ]  = QPen(Qt::full_name, 0);

#define ADD_DEFAULT_PEN2(full_name, short_name) \
    _pens[ #full_name ]  = QPen(Qt::full_name, 0); \
    _pens[ #short_name ] = QPen(Qt::full_name, 0);

#define ADD_DEFAULT_BRUSH_AND_PEN(full_name) \
    ADD_DEFAULT_BRUSH(full_name) \
    ADD_DEFAULT_PEN(full_name)

#define ADD_DEFAULT_BRUSH_AND_PEN2(full_name, short_name) \
    ADD_DEFAULT_BRUSH2(full_name, short_name) \
    ADD_DEFAULT_PEN2(full_name, short_name)

    // Default brush and pen
    _brushes[QString()] = QBrush();
    _pens[QString()] = QPen(Qt::black, 0);

    // Named brushes and pens
    ADD_DEFAULT_BRUSH_AND_PEN2(transparent, none);

    ADD_DEFAULT_BRUSH_AND_PEN2(cyan, c);
    ADD_DEFAULT_BRUSH_AND_PEN2(yellow, y);
    ADD_DEFAULT_BRUSH_AND_PEN2(magenta, m);
    ADD_DEFAULT_BRUSH_AND_PEN2(red, r);
    ADD_DEFAULT_BRUSH_AND_PEN2(green, g);
    ADD_DEFAULT_BRUSH_AND_PEN2(blue, b);
    ADD_DEFAULT_BRUSH_AND_PEN2(black, k);
    ADD_DEFAULT_BRUSH_AND_PEN2(white, w);
    ADD_DEFAULT_BRUSH_AND_PEN(darkGray);
    ADD_DEFAULT_BRUSH_AND_PEN(gray);
    ADD_DEFAULT_BRUSH_AND_PEN(lightGray);
    ADD_DEFAULT_BRUSH_AND_PEN(darkCyan);
    ADD_DEFAULT_BRUSH_AND_PEN(darkYellow);
    ADD_DEFAULT_BRUSH_AND_PEN(darkMagenta);
    ADD_DEFAULT_BRUSH_AND_PEN(darkRed);
    ADD_DEFAULT_BRUSH_AND_PEN(darkGreen);
    ADD_DEFAULT_BRUSH_AND_PEN(darkBlue);
}

VibesGraphicsItem::VibesGraphicsItem(QGraphicsItem *qGraphicsItem)
: _qGraphicsItem(qGraphicsItem), _nbDim(0), _dimX(-1), _dimY(-1)
{
}

bool VibesGraphicsItem::setJson(QJsonObject json, int dimX, int dimY)
{
    if (!parseJson(json)) return false;
    _json = json;

    setProj(dimX, dimY);
    return true;
}

QJsonValue VibesGraphicsItem::jsonValue(const QString& key) const
{
    // If object has the requested property, return it
    if (json().contains(key))
    {
        return json()[key];
    }
        // Else, return the property from its parent group
    else
    {
        const VibesGraphicsGroup* group = 0;
        if (_qGraphicsItem)
            group = qgraphicsitem_cast<const VibesGraphicsGroup*>(_qGraphicsItem->parentItem());
        // Item is member of a group, ask the group for the requested property
        if (group)
        {
            return group->jsonValue(key);
        }
            // Item has no parent group, then return an empty value
        else
        {
            return QJsonValue();
        }
    }
}

void VibesGraphicsItem::setJsonValue(const QString &key, const QJsonValue &value)
{
    // Wrap key-value pair in a QJsonObject...
    QJsonObject jsonObject;
    jsonObject[key] = value;
    // ...and call setJsonValues
    setJsonValues(jsonObject);
}

void VibesGraphicsItem::setJsonValues(const QJsonObject &values)
{
    bool bNeedProjection = false;

    for (QJsonObject::const_iterator prop = values.constBegin(); prop != values.constEnd(); prop++)
    {
        // Cannot update read-only properties
        if (propertyIsReadOnly(prop.key()))
            continue;
        // Set or update property value
        _json[prop.key()] = prop.value();

        // Check if we need to update projection
        if (propertyChangesGeometry(prop.key()))
            bNeedProjection = true;
    }

    // Update graphics with new Json
    parseJson(_json);

    if (bNeedProjection)
        computeProjection(_dimX, _dimY);

    // Apply property change
    if (this->_qGraphicsItem->type() != VibesGraphicsGroupType)
    {
        this->updateProj();
    }
}

bool VibesGraphicsItem::setProj(int dimX, int dimY)
{
    _dimX = dimX;
    _dimY = dimY;
    if (existsInProj(_dimX, _dimY))
    {
        return computeProjection(_dimX, _dimY);
    }
    else
    {
        /// \todo Change visibility to none when projection is not available
        return false;
    }
}

VibesGraphicsItem * VibesGraphicsItem::newWithType(const QString type)
{
    if (type == "group")
    {
        return new VibesGraphicsGroup();
    }
    else if (type == "box")
    {
        return new VibesGraphicsBox();
    }
    else if (type == "boxes")
    {
        return new VibesGraphicsBoxes();
    }
    else if (type == "boxes union")
    {
        return new VibesGraphicsBoxesUnion();
    }
    else if (type == "ellipse")
    {
        return new VibesGraphicsEllipse();
    }
    else if (type == "pie")
    {
        return new VibesGraphicsPie();
    }
    else if (type == "ring")
    {
        return new VibesGraphicsRing();
    }
    else if (type == "point")
    {
        return new VibesGraphicsPoint();
    }
    else if (type == "points")
    {
        return new VibesGraphicsPoints();
    }
    else if (type == "line")
    {
        return new VibesGraphicsLine();
    }
    else if (type == "arrow")
    {
        return new VibesGraphicsArrow();
    }
    else if (type == "polygon")
    {
        return new VibesGraphicsPolygon();
    }
    else if (type == "vehicle")
    {
        return new VibesGraphicsVehicle();
    }
    else if (type == "vehicle_auv")
    {
        return new VibesGraphicsVehicleAUV();
    }
    else if (type == "vehicle_tank")
    {
        return new VibesGraphicsVehicleTank();
    }
    else if (type == "raster")
    {
        return new VibesGraphicsRaster();
    }
    return 0;
}

bool VibesGraphicsItem::parseJson(QJsonObject &json)
{
    // Process format string
    if (json.contains("format") && json["format"].isString())
    {
        QString format = json["format"].toString();
        format = format.trimmed();
        // Extract FaceColor
        int fcStart = format.indexOf('[');
        int fcEnd = format.lastIndexOf(']');
        if (fcStart < fcEnd)
        {
            json["FaceColor"] = QJsonValue(format.mid(fcStart + 1, fcEnd - fcStart - 1).trimmed());
            format.remove(fcStart, fcEnd - fcStart + 1);
        }
        
        // Extract EdgeColor
        format = format.trimmed();
        if (!format.isEmpty())
        {
            json["EdgeColor"] = QJsonValue(format);
        }
        // Remove "format" from Json
        json.remove("format");
    }

    // Process object name
    if (json.contains("name") && json["name"].isString())
    {
        this->setName(json["name"].toString());
    }

    // Process object linewidth
    if (json.contains("linewidth") )
    {
        json["LineWidth"] = QJsonValue(json["linewidth"]);
        json.remove("linewidth");
    }

    // Process object linestyle
    if (json.contains("linestyle") )
    {
        json["LineStyle"] = QJsonValue(json["linestyle"]);
        json.remove("linestyle");
    }

    // Process object specific JSON
    return parseJsonGraphics(json);
}

bool VibesGraphicsItem::isJsonMatrix(const QJsonValue value, int &nbRows, int &nbCols)
{
    nbRows = 0;
    nbCols = 0;
    // Check number of rows
    if (!value.isArray()) return false;
    QJsonArray lines = value.toArray();
    nbRows = lines.size();
    if (nbRows == 0) return false;
    // Check number of columns and consistency among rows
    QJsonArray::const_iterator line = lines.begin();
    if (!(*line).isArray()) return false;
    nbCols = (*line).toArray().size();
    if (nbCols == 0) return false;
    while (++line != lines.end())
    {
        if (!(*line).isArray()) return false;
        if ((*line).toArray().size() != nbCols) return false;
    }
    return true;
}

void VibesGraphicsGroup::addToGroup(VibesGraphicsItem *item)
{
    // Cannot add nullptr to the group
    if (!item) return;
    // Add to the group
    QGraphicsItemGroup::addToGroup(vibesgraphicsitem_cast<QGraphicsItem*>(item));
    // Update dimension
    this->_nbDim = qMax(_nbDim, item->dimension());
    // Update item with group properties
    if (VibesGraphicsItem::scene())
    {
        item->setProj(VibesGraphicsItem::scene()->dimX(), VibesGraphicsItem::scene()->dimY());
    }
}

void VibesGraphicsGroup::clear()
{
    // Remove all children
    QList<QGraphicsItem*> children = this->childItems();

    foreach(QGraphicsItem* child, children)
    {
        VibesGraphicsItem * item = qgraphicsitem_cast<VibesGraphicsItem *>(child);
        if (item) delete item;
    }
}

bool VibesGraphicsGroup::parseJsonGraphics(const QJsonObject &json)
{
    // Set graphical properties
    QList<QGraphicsItem*> children = this->childItems();

    foreach(QGraphicsItem* child, children)
    {
        VibesGraphicsItem * item = qgraphicsitem_cast<VibesGraphicsItem *>(child);
        // Each item gets the group properties if needed
        if (item)
        {
            if (json["EdgeColor"].toString()!=QString())
            {
                item->setJsonValue("EdgeColor",json["EdgeColor"]);
            }
            if (json["FaceColor"].toString()!=QString())
            {
                item->setJsonValue("FaceColor",json["FaceColor"]);
            }
            if (json["LineStyle"].toString()!=QString())
            {
                item->setJsonValue("LineStyle",json["LineStyle"]);
            }
            if (json["LineWidth"].toString()!=QString())
            {
                item->setJsonValue("LineWidth",json["LineWidth"]);
            }
            item->updateProj();
        }
            
    }
    return true;
}

bool VibesGraphicsBox::parseJsonGraphics(const QJsonObject &json)
{
    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    if (json.contains("type"))
    {
        // Retrieve type
        QString type = json["type"].toString();

        // VibesGraphicsBox has JSON type "box"
        if (type == "box")
        {
            QJsonArray bounds = json["bounds"].toArray();
            // We need at least a 2-D box. Number of bounds has to be even.
            if (bounds.size() < 4 || bounds.size() % 2 != 0)
                return false;

            // Compute dimension
            this->_nbDim = bounds.size() / 2;

            // Set graphical properties
            this->setPen(vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString()));
            this->setBrush(vibesDefaults.brush(jsonValue("FaceColor").toString()));

            // Update successful
            return true;
        }
    }

    // Unknown or empty JSON, update failed
    return false;
}

bool VibesGraphicsBox::computeProjection(int dimX, int dimY)
{
    const QJsonObject & json = this->_json;

    // Get shape color (or default if not specified)
    const QBrush & brush = vibesDefaults.brush(jsonValue("FaceColor").toString());
    const QPen & pen = vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString());

    Q_ASSERT(json.contains("type"));
    Q_ASSERT(json["type"].toString() == "box");

    QJsonArray bounds = json["bounds"].toArray();
    // Compute dimension
    Q_ASSERT(this->_nbDim == bounds.size() / 2);
    Q_ASSERT(bounds.size() >= (2 * (qMax(dimX, dimY) + 1)));

    // Read bounds
    double lb_x = bounds[2 * dimX].toDouble();
    double ub_x = bounds[2 * dimX + 1].toDouble();
    double lb_y = bounds[2 * dimY].toDouble();
    double ub_y = bounds[2 * dimY + 1].toDouble();

    // Update rectangle
    this->setRect(lb_x, lb_y, ub_x - lb_x, ub_y - lb_y);

    // Update rectangle color
    this->setPen(pen);
    this->setBrush(brush);
    QString msg;// = QString("[%1, %2]x[%3, %4]").arg(lb_x).arg(ub_x).arg(lb_y).arg(ub_y);
#if QT_VERSION >= 0x050600 // Qt 5.6 or over
    msg = QString::asprintf("x: [%f ; %f]\ny: [%f ; %f]\n %f x %f", lb_x, ub_x, lb_y, ub_y, ub_x - lb_x, ub_y - lb_y);
#else
    msg.sprintf("x: [%f ; %f]\ny: [%f ; %f]\n %f x %f", lb_x, ub_x, lb_y, ub_y, ub_x - lb_x, ub_y - lb_y);
#endif
    this->setToolTip(msg);

    // Update successful
    return true;
}

//
// VibesGraphicsBoxes
//

bool VibesGraphicsBoxes::parseJsonGraphics(const QJsonObject &json)
{
    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    if (json.contains("type"))
    {
        // Retrieve type
        QString type = json["type"].toString();

        // VibesGraphicsBoxes has JSON type "boxes"
        if (type == "boxes")
        {
            // Check that the "bounds" fields is a matrix
            int nbCols, nbRows;
            if (!isJsonMatrix(json["bounds"], nbRows, nbCols))
                return false;
            // Number of bounds has to be even
            if (nbCols % 2 != 0)
                return false;
            // Number of bounds has to be at least 4 (to draw boxes)
            if (nbCols < 4)
                return 0;

            //            bool colors = json.contains("colors");
            //            bool enoughColors = false;
            //            if (colors)
            //                enoughColors = json["colors"].toArray().size() == nbRows;

            // Compute dimension
            this->_nbDim = nbCols / 2;

            // Set graphical properties
            QList<QGraphicsItem*> rects = this->childItems();

            foreach(QGraphicsItem* item, rects)
            {
                QGraphicsRectItem * rect = qgraphicsitem_cast<QGraphicsRectItem *>(item);
                if (!rect) continue;
                rect->setPen(vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString()));
                rect->setBrush(vibesDefaults.brush(jsonValue("FaceColor").toString()));
            }

            // Update successful
            return true;
        }
    }

    // Unknown or empty JSON, update failed
    return false;
}

bool VibesGraphicsBoxes::computeProjection(int dimX, int dimY)
{
    const QJsonObject & json = this->_json;

    // Get shape color (or default if not specified)
    const QBrush & brush = vibesDefaults.brush(jsonValue("FaceColor").toString());
    const QPen & pen = vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString());

    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    Q_ASSERT(json.contains("type"));
    // VibesGraphicsBoxes has JSON type "boxes"
    Q_ASSERT(json["type"].toString() == "boxes");

    // VibesGraphicsBoxes is basically a QGraphicsItemGroup containing QGraphicsRectItems
    // Before update, we first remove all existing rectangles
    {
        QList<QGraphicsItem*> rects = this->childItems();
        foreach(QGraphicsItem* rect, rects) delete rect;
    }
    // "bounds" is a matrix
    Q_ASSERT(isJsonMatrix(json["bounds"]));

    // Fill group with projected boxes

    foreach(const QJsonValue value, json["bounds"].toArray())
    {
        const QJsonArray box = value.toArray();
        // Read bounds
        double lb_x = box[2 * dimX].toDouble();
        double ub_x = box[2 * dimX + 1].toDouble();
        double lb_y = box[2 * dimY].toDouble();
        double ub_y = box[2 * dimY + 1].toDouble();
        // Create a new rect
        QGraphicsRectItem * rect = new QGraphicsRectItem(lb_x, lb_y, ub_x - lb_x, ub_y - lb_y);
        rect->setPen(pen);
        rect->setBrush(brush);
        // Add it to the group
        this->addToGroup(rect);
    }

    /*
        QJsonArray boundsX_lb = json["boundsX_lb"].toArray();
        QJsonArray boundsX_ub = json["boundsX_ub"].toArray();
        QJsonArray boundsY_lb = json["boundsY_lb"].toArray();
        QJsonArray boundsY_ub = json["boundsY_ub"].toArray();

        if (boundsX_lb.size() == boundsX_ub.size() &&
                boundsX_ub.size() == boundsY_lb.size() &&
                boundsY_lb.size() == boundsY_ub.size())
        {
            bool colors = json.contains("colors");
            bool enoughColors = false;
            if (colors)
                enoughColors = json["colors"].toArray().size() == boundsX_lb.size();
            for (int i = 0; i < boundsX_lb.size(); i++)
            {
                // Read bounds
                double lb_x = boundsX_lb[i].toDouble();
                double ub_x = boundsX_ub[i].toDouble();
                double lb_y = boundsY_lb[i].toDouble();
                double ub_y = boundsY_ub[i].toDouble();
                // Create a new rect
                QGraphicsRectItem * rect = new QGraphicsRectItem(lb_x, lb_y, ub_x - lb_x, ub_y - lb_y);
                rect->setPen(pen);
                rect->setBrush(brush);
                // Add it to the group
                this->addToGroup( rect );
            }
     */

    // Update successful
    return true;
}


//
// VibesGraphicsBoxesUnion
//

bool VibesGraphicsBoxesUnion::parseJsonGraphics(const QJsonObject &json)
{
    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    if (json.contains("type"))
    {
        // Retrieve type
        QString type = json["type"].toString();

        // VibesGraphicsBoxesUnion has JSON type "boxes union"
        if (type == "boxes union")
        {
            // Check that the "bounds" fields is a matrix
            int nbCols, nbRows;
            if (!isJsonMatrix(json["bounds"], nbRows, nbCols))
                return false;
            // Number of bounds has to be even
            if (nbCols % 2 != 0)
                return false;
            // Number of bounds has to be at least 4 (to draw boxes)
            if (nbCols < 4)
                return 0;

            //            bool colors = json.contains("colors");
            //            bool enoughColors = false;
            //            if (colors)
            //                enoughColors = json["colors"].toArray().size() == nbRows;

            // Compute dimension
            this->_nbDim = nbCols / 2;

            // Set graphical properties
            this->setPen(vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString()));
            this->setBrush(vibesDefaults.brush(jsonValue("FaceColor").toString()));

            // Update successful
            return true;
        }
    }

    // Unknown or empty JSON, update failed
    return false;
}

bool VibesGraphicsBoxesUnion::computeProjection(int dimX, int dimY)
{
    const QJsonObject & json = this->_json;

    // Get shape color (or default if not specified)
    const QBrush & brush = vibesDefaults.brush(jsonValue("FaceColor").toString());
    const QPen & pen = vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString());

    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    Q_ASSERT(json.contains("type"));
    // VibesGraphicsBoxes has JSON type "boxes union"
    Q_ASSERT(json["type"].toString() == "boxes union");
    // "bounds" is a matrix
    Q_ASSERT(isJsonMatrix(json["bounds"]));

    // Update path with projected boxes
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);

    foreach(const QJsonValue value, json["bounds"].toArray())
    {
        const QJsonArray box = value.toArray();
        // Read bounds
        double lb_x = box[2 * dimX].toDouble();
        double ub_x = box[2 * dimX + 1].toDouble();
        double lb_y = box[2 * dimY].toDouble();
        double ub_y = box[2 * dimY + 1].toDouble();
        // Create a new rect path
        QPainterPath rect_path;
        rect_path.addRect(lb_x, lb_y, ub_x - lb_x, ub_y - lb_y);
        // Unite with current path
        path |= rect_path;
    }
    this->setPath(path);
    // Set graphics properties
    this->setPen(pen);
    this->setBrush(brush);

    // Update successful
    return true;
}


//
// VibesGraphicsEllipse
//

bool VibesGraphicsEllipse::parseJsonGraphics(const QJsonObject &json)
{
    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    if (json.contains("type"))
    {
        // Retrieve type
        QString type = json["type"].toString();

        // JSON type for VibesGraphicsEllipse is "ellipse" (Ellipses and Confidence ellipses)
        if (type == "ellipse")
        {
            QJsonArray center = json["center"].toArray();
            if (center.size() >= 2)
            {
                if (json.contains("axis") && json.contains("orientation"))
                {
                    // Axis-orientation only supported for 2D ellipses
                    if (center.size() != 2) return false;
                    if (json["axis"].toArray().size() != 2) return false;

                }
                else if (json.contains("angles"))
                {
                    if (json["angles"].toArray().size() != 2) return false;
                }
                else if (json.contains("covariance"))
                {
                    // A covariance matrix has been provided (Confidence ellipse)
                    double k = json.contains("sigma") ? json["sigma"].toDouble() : 5;
                    QJsonArray covariance = json["covariance"].toArray();
                    if (covariance.size() != pow(center.size(), 2)) return false;
                }
                else
                {
                    // Should not be here, ellipse parameters have not been provided
                    return false;
                }
                // Set dimension
                this->_nbDim = center.size();

                // Set graphical properties
                this->setPen(vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString()));
                this->setBrush(vibesDefaults.brush(jsonValue("FaceColor").toString()));

                // Update successful
                return true;
            }
        }
    }

    // Unknown or empty JSON, update failed
    return false;
}

bool VibesGraphicsEllipse::computeProjection(int dimX, int dimY)
{
    const QJsonObject & json = this->_json;

    // Get shape color (or default if not specified)
    const QBrush & brush = vibesDefaults.brush(jsonValue("FaceColor").toString());
    const QPen & pen = vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString());

    Q_ASSERT(json.contains("type"));
    Q_ASSERT(json["type"].toString() == "ellipse");

    QJsonArray center = json["center"].toArray();

    Q_ASSERT(center.size() >= 2);
    Q_ASSERT(this->_nbDim == center.size());
    Q_ASSERT(center.size() >= (qMax(dimX, dimY) + 1));

    // Get center
    double x = center[dimX].toDouble();
    double y = center[dimY].toDouble();

    // Semi-major and semi-minor axes, and rotation
    double wx, wy, angle;
    // Intialize with full-ellipse
    int angle_min = 0;
    int angle_max = 5760;

    Q_ASSERT((json.contains("axis") && json.contains("orientation")) || json.contains("covariance"));
    if (json.contains("axis") && json.contains("orientation"))
    {
        // Semi-major and semi-minor axes are directly provided (Ellipse or circle)
        Q_ASSERT((dimX == 0 && dimY == 1) || (dimX == 1 && dimY == 0));

        QJsonArray axis = json["axis"].toArray();
        wx = axis[0].toDouble();
        wy = axis[1].toDouble();
        if (dimX == 0 && dimY == 1)
            angle = json.value("orientation").toDouble();
        else if (dimX == 1 && dimY == 0)
            angle = 90.0 - json.value("orientation").toDouble();
        else
            return false;
    }
    else if (json.contains("covariance"))
    {
        // A covariance matrix has been provided (Confidence ellipse)
        double k = json.contains("sigma") ? json["sigma"].toDouble() : 5;
        QJsonArray covariance = json["covariance"].toArray();
        Q_ASSERT(covariance.size() == center.size() * center.size());
        double sxx = covariance[dimX + center.size() * dimX].toDouble();
        double sxy = covariance[dimX + center.size() * dimY].toDouble();
        double syy = covariance[dimY + center.size() * dimY].toDouble();
        // Compute the semi-major and semi-minor axes and rotation angle.
        axisAngleFromCovarianceK(sxx, syy, sxy, k, wx, wy, angle);
    }
    else
    {
        // Should not be here, ellipse parameters have not been provided
        return false;
    }

    if (json.contains("angles"))
    {
        QJsonArray bounds = json["angles"].toArray();
        angle_min = static_cast<int> (bounds[0].toDouble()*16);
        angle_max = static_cast<int> (bounds[1].toDouble()*16);
    }
    // Update ellipse
    this->setRect(-wx, -wy, 2 * wx, 2 * wy);
    this->setRotation(angle);
    this->setPos(x, y);
    this->setStartAngle(angle_min);
    this->setSpanAngle(angle_max - angle_min);
    // Update ellipse properties
    this->setPen(pen);
    this->setBrush(brush);
    // Update successful
    return true;
}

void VibesGraphicsEllipse::axisAngleFromCovarianceK(const double &sxx, const double &syy,
                                                    const double &sxy, const double &k,
                                                    double &wx, double &wy, double &angle)
{
    double eval1, eval2, det, trace, rightTerm;
    double evect1[2], evect2[2];

    if (sxy == 0)
    {
        // Uncorrelated case
        eval1 = sxx;
        eval2 = syy;
        evect1[0] = 1;
        evect1[1] = 0;
        evect2[0] = 0;
        evect2[1] = 1;
    }
    else
    {
        // Correlated case
        det = sxx * syy - pow(sxy, 2);
        trace = sxx + syy;
        rightTerm = sqrt(pow(sxx + syy, 2) / 4 - det);
        eval1 = trace / 2 + rightTerm;
        eval2 = trace / 2 - rightTerm;

        evect1[0] = evect2[0] = 1; // We set the x-component of the eigenvectors to 1
        evect1[1] = (eval1 - sxy - sxx) / (sxy + syy - eval1);
        evect2[1] = (eval2 - sxy - sxx) / (sxy + syy - eval2);
    }

    // (evect1; evect2) give us the rotation matrix
    // k*sqrt(eval1) k*sqrt(eval2) give us the main axis-sizes of the ellipse
    angle = (evect1[0] != evect1[0]) || (evect1[1] != evect1[1]) ? (atan2(evect2[1], evect2[0])*180 * M_1_PI - 90) : atan2(evect1[1], evect1[0])*180 * M_1_PI;
    wx = k * sqrt(eval1);
    wy = k * sqrt(eval2);
}

//
// VibesGraphicsBoxesUnion
//

bool VibesGraphicsLine::parseJsonGraphics(const QJsonObject &json)
{
    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    if (json.contains("type"))
    {
        // Retrieve type
        QString type = json["type"].toString();

        // VibesGraphicsLine has JSON type "line"
        if (type == "line")
        {
            // Check that the "points" field is a matrix
            int nbCols, nbRows;
            if (!isJsonMatrix(json["points"], nbRows, nbCols))
                return false;
            // Number of coordinates has to be at least 2 (to draw in the plane)
            if (nbCols < 2)
                return 0;

            // Compute dimension
            this->_nbDim = nbCols;

            // Set pen
            this->setPen(vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString()));

            // Update successful
            return true;
        }
    }

    // Unknown or empty JSON, update failed
    return false;
}

bool VibesGraphicsLine::computeProjection(int dimX, int dimY)
{
    const QJsonObject & json = this->_json;

    // Get shape color (or default if not specified)
    const QPen & pen = vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString());

    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    Q_ASSERT(json.contains("type"));
    // VibesGraphicsLine has JSON type "line"
    Q_ASSERT(json["type"].toString() == "line");
    // "bounds" is a matrix
    Q_ASSERT(isJsonMatrix(json["points"]));

    // Update line with projected points
    QPainterPath path;
    QPolygonF polygon;

    foreach(const QJsonValue value, json["points"].toArray())
    {
        // Read coordinates and append them to the list of points
        const QJsonArray coords = value.toArray();
        polygon << QPointF(coords[dimX].toDouble(), coords[dimY].toDouble());
    }
    // Update polygon with the list of vertices
    path.addPolygon(polygon);
    this->setPath(path);
    // Set graphics properties
    this->setPen(pen);
    //this->setBrush(brush);

    // Update successful
    return true;
}

bool VibesGraphicsPolygon::parseJsonGraphics(const QJsonObject &json)
{
    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    if (json.contains("type"))
    {
        // Retrieve type
        QString type = json["type"].toString();

        // VibesGraphicsBox has JSON type "polygon"
        if (type == "polygon")
        {
            // Check that the "points" field is a matrix
            int nbCols, nbRows;
            if (!isJsonMatrix(json["bounds"], nbRows, nbCols))
                return false;
            // Number of coordinates has to be at least 2 (to draw in the plane)
            if (nbCols < 2)
                return 0;

            // Compute dimension
            this->_nbDim = nbCols;

            // Set pen
            this->setPen(vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString()));
            this->setBrush(vibesDefaults.brush(jsonValue("FaceColor").toString()));

            // Update successful
            return true;
        }
    }

    // Unknown or empty JSON, update failed
    return false;
}

bool VibesGraphicsPolygon::computeProjection(int dimX, int dimY)
{
    const QJsonObject & json = this->_json;

    // Get shape color (or default if not specified)
    const QBrush & brush = vibesDefaults.brush(jsonValue("FaceColor").toString());
    const QPen & pen = vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString());

    Q_ASSERT(json.contains("type"));
    Q_ASSERT(json["type"].toString() == "polygon");

    // Update polygon
    QPolygonF polygon;

    foreach(const QJsonValue value, json["bounds"].toArray())
    {
        // Read coordinates and append them to the list of points
        const QJsonArray coords = value.toArray();
        polygon << QPointF(coords[dimX].toDouble(), coords[dimY].toDouble());
    }
    this->setPolygon(polygon);

    // Update polygon color
    this->setPen(pen);
    this->setBrush(brush);

    // Update successful
    return true;
}


//
// VibesGraphicsVehicle
//

bool VibesGraphicsVehicle::parseJsonGraphics(const QJsonObject &json)
{
    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    if (json.contains("type"))
    {
        // Retrieve type
        QString type = json["type"].toString();

        // JSON type for VibesGraphicsVehicle is "vehicle"
        if (type == "vehicle")
        {
            if (json.contains("center") && json.contains("length") && json.contains("orientation"))
            {
                int center_size = json["center"].toArray().size();
                if (center_size == 2 && json["length"].toDouble() > 0.)
                {
                    // Set dimension
                    this->_nbDim = center_size;

                    // Update successful
                    return true;
                }
            }
        }
    }

    // Unknown or empty JSON, update failed
    return false;
}

bool VibesGraphicsVehicle::computeProjection(int dimX, int dimY)
{
    const QJsonObject & json = this->_json;

    // Get shape color (or default if not specified)
    const QBrush & brush = vibesDefaults.brush(jsonValue("FaceColor").toString());
    const QPen & pen = vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString());

    Q_ASSERT(json.contains("type"));
    Q_ASSERT(json["type"].toString() == "vehicle");

    QJsonArray center = json["center"].toArray();
    double length = json["length"].toDouble();
    double orientation = json["orientation"].toDouble();

    Q_ASSERT(center.size() == 2);
    Q_ASSERT(this->_nbDim == center.size());
    Q_ASSERT(length > 0.);

    // Get center
    const QPointF & centerPoint = QPointF(center[dimX].toDouble(), center[dimY].toDouble());

    // update child items if they exist
    if (this->childItems().size() > 0)
    {
        foreach(QGraphicsItem * item, this->childItems())
        {
            //to vibes graphics item
            QGraphicsPolygonItem *graphics_polygon = qgraphicsitem_cast<QGraphicsPolygonItem *>(item);
            graphics_polygon->setPen(pen);
            graphics_polygon->setBrush(brush);
            graphics_polygon->setTransformOriginPoint(centerPoint);
            graphics_polygon->setRotation(orientation);
            graphics_polygon->setScale(length / 4.); // initial vehicle's length is 4
        }
    }
    else{

        // Set polygon shape
        QPolygonF polygon;
        polygon << QPointF(-1. * length, 1. * length) + centerPoint;
        polygon << QPointF(+3. * length, 0. * length) + centerPoint;
        polygon << QPointF(-1. * length, -1. * length) + centerPoint;


        // Draw with the new properties
        QGraphicsPolygonItem *graphics_polygon = new QGraphicsPolygonItem(polygon);
        graphics_polygon->setPen(pen);
        graphics_polygon->setBrush(brush);
        graphics_polygon->setTransformOriginPoint(centerPoint);
        graphics_polygon->setRotation(orientation);
        graphics_polygon->setScale(length / 4.); // initial vehicle's length is 4
        this->addToGroup(graphics_polygon);
    }

    // Update successful
    return true;
}


//
// VibesGraphicsVehicleAUV
//

bool VibesGraphicsVehicleAUV::parseJsonGraphics(const QJsonObject &json)
{
    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    if (json.contains("type"))
    {
        // Retrieve type
        QString type = json["type"].toString();

        // JSON type for VibesGraphicsVehicleAUV is "vehicle_auv"
        if (type == "vehicle_auv")
        {
            if (json.contains("center") && json.contains("length") && json.contains("orientation"))
            {
                int center_size = json["center"].toArray().size();
                if (center_size == 2 && json["length"].toDouble() > 0.)
                {
                    // Set dimension
                    this->_nbDim = center_size;

                    // Update successful
                    return true;
                }
            }
        }
    }

    // Unknown or empty JSON, update failed
    return false;
}

bool VibesGraphicsVehicleAUV::computeProjection(int dimX, int dimY)
{
    const QJsonObject & json = this->_json;

    // Get shape color (or default if not specified)
    const QBrush & brush = vibesDefaults.brush(jsonValue("FaceColor").toString());
    const QPen & pen = vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString());

    Q_ASSERT(json.contains("type"));
    Q_ASSERT(json["type"].toString() == "vehicle_auv");

    QJsonArray center = json["center"].toArray();
    double length = json["length"].toDouble();
    double orientation = json["orientation"].toDouble();

    Q_ASSERT(center.size() == 2);
    Q_ASSERT(this->_nbDim == center.size());
    Q_ASSERT(length > 0.);

    // Get center
    const QPointF & centerPoint = QPointF(center[dimX].toDouble(), center[dimY].toDouble());

    /*  This shape is inspired by the MOOS middleware GUI (see pMarineViewer)   */

    // update child items if they exist
    if (this->childItems().size() > 0)
    {
        foreach(QGraphicsItem * item, this->childItems())
        {
            //to vibes graphics item
            QGraphicsPolygonItem *graphics_polygon = qgraphicsitem_cast<QGraphicsPolygonItem *>(item);
            graphics_polygon->setPen(pen);
            graphics_polygon->setBrush(brush);
            graphics_polygon->setTransformOriginPoint(centerPoint);
            graphics_polygon->setRotation(orientation);
            graphics_polygon->setScale(length / 7.); // initial vehicle's length is 4
        }
    }

    else{
        // Set body shape
        {
            QPolygonF body;
            body << QPointF(-4. * length, 0. * length) + centerPoint;
            body << QPointF(-2. * length, 1. * length) + centerPoint;
            body << QPointF(2. * length, 1. * length) + centerPoint;

            for (float i = 90.; i > -90.; i -= 10.) // noise
                body << QPointF((cos(i * M_PI / 180.0) + 2.) * length,
                                (sin(i * M_PI / 180.0) + 0.) * length) + centerPoint;

            body << QPointF(2. * length, -1. * length) + centerPoint;
            body << QPointF(-2. * length, -1. * length) + centerPoint;


            // Draw with the new properties
            QGraphicsPolygonItem *graphics_body = new QGraphicsPolygonItem(body);
            graphics_body->setPen(pen);
            graphics_body->setBrush(brush);
            graphics_body->setTransformOriginPoint(centerPoint);
            graphics_body->setRotation(orientation);
            graphics_body->setScale(length / 7.); // initial vehicle's length is 7
            this->addToGroup(graphics_body);
            graphics_body->setPen(pen);
        }

        // Set propulsion unit shape
        {
            QPolygonF propunit;
            propunit << QPointF(-4. * length, 1 * length) + centerPoint;
            propunit << QPointF(-3.25 * length, 1 * length) + centerPoint;
            propunit << QPointF(-3.25 * length, -1 * length) + centerPoint;
            propunit << QPointF(-4. * length, -1 * length) + centerPoint;

            // Draw with the new properties
            QGraphicsPolygonItem *graphics_propunit = new QGraphicsPolygonItem(propunit);
            graphics_propunit->setPen(pen);
            graphics_propunit->setBrush(brush);
            graphics_propunit->setTransformOriginPoint(centerPoint);
            graphics_propunit->setRotation(orientation);
            graphics_propunit->setScale(length / 7.); // initial vehicle's length is 7
            this->addToGroup(graphics_propunit);
        }
    }



    // Update successful
    return true;
}


//
// VibesGraphicsVehicleTank
//

bool VibesGraphicsVehicleTank::parseJsonGraphics(const QJsonObject &json)
{
    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    if (json.contains("type"))
    {
        // Retrieve type
        QString type = json["type"].toString();

        // JSON type for VibesGraphicsVehicleTank is "vehicle_tank"
        if (type == "vehicle_tank")
        {
            if (json.contains("center") && json.contains("length") && json.contains("orientation"))
            {
                int center_size = json["center"].toArray().size();
                if (center_size == 2 && json["length"].toDouble() > 0.)
                {
                    // Set dimension
                    this->_nbDim = center_size;

                    // Update successful
                    return true;
                }
            }
        }
    }

    // Unknown or empty JSON, update failed
    return false;
}
#include <iostream>
bool VibesGraphicsVehicleTank::computeProjection(int dimX, int dimY)
{
    const QJsonObject & json = this->_json;

    // Get shape color (or default if not specified)
    const QBrush & brush = vibesDefaults.brush(jsonValue("FaceColor").toString());
    const QPen & pen = vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString());

    Q_ASSERT(json.contains("type"));
    Q_ASSERT(json["type"].toString() == "vehicle_tank");

    QJsonArray center = json["center"].toArray();
    double length = json["length"].toDouble();
    double orientation = json["orientation"].toDouble();

    Q_ASSERT(center.size() == 2);
    Q_ASSERT(this->_nbDim == center.size());
    Q_ASSERT(length > 0.);

    // Get center
    const QPointF & centerPoint = QPointF(center[dimX].toDouble(), center[dimY].toDouble());

    /*  This shape is inspired by Luc Jaulin   */

    // update child items if they exist
    if (this->childItems().size() > 0)
    {
        foreach(QGraphicsItem * item, this->childItems())
        {
            //to vibes graphics item
            QGraphicsPolygonItem *graphics_polygon = qgraphicsitem_cast<QGraphicsPolygonItem *>(item);
            graphics_polygon->setPen(pen);
            graphics_polygon->setBrush(brush);
            graphics_polygon->setTransformOriginPoint(centerPoint);
            graphics_polygon->setRotation(orientation);
            graphics_polygon->setScale(length / 4.); // initial vehicle's length is 4
        }
    }
    else{

        // Set body shape
        {
            QPolygonF body;
            body << QPointF(1., -1.5) + centerPoint;
            body << QPointF(-1., -1.5) + centerPoint;
            body << QPointF(0., -1.5) + centerPoint;
            body << QPointF(0., -1.) + centerPoint;
            body << QPointF(-1., -1.) + centerPoint;
            body << QPointF(-1., 1.) + centerPoint;
            body << QPointF(0., 1.) + centerPoint;
            body << QPointF(0., 1.5) + centerPoint;
            body << QPointF(-1., 1.5) + centerPoint;
            body << QPointF(1., 1.5) + centerPoint;
            body << QPointF(0., 1.5) + centerPoint;
            body << QPointF(0., 1.) + centerPoint;
            body << QPointF(3., 0.5) + centerPoint;
            body << QPointF(3., -0.5) + centerPoint;
            body << QPointF(0., -1.) + centerPoint;
            body << QPointF(0., -1.5) + centerPoint;


            // Draw with the new properties
            QGraphicsPolygonItem *graphics_body = new QGraphicsPolygonItem(body);
            graphics_body->setPen(pen);
            graphics_body->setBrush(brush);
            graphics_body->setTransformOriginPoint(centerPoint);
            graphics_body->setRotation(orientation);

            graphics_body->setScale(length/4.); // initial vehicle's length is 4
            this->addToGroup(graphics_body);
        }
    }

    // Update successful
    return true;
}


//
// VibesGraphicsArrow
//

bool VibesGraphicsArrow::parseJsonGraphics(const QJsonObject &json)
{
    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    if (json.contains("type"))
    {
        // Retrieve type
        QString type = json["type"].toString();

        // VibesGraphicsArrow has JSON type "arrow"
        if (type == "arrow")
        {
            // Check that the "points" field is a matrix
            int nbCols, nbRows;
            if (!isJsonMatrix(json["points"], nbRows, nbCols))
                return false;
            if (json["tip_length"].toDouble() < 0)
                return false;
            // Number of coordinates has to be at least 2 (to draw in the plane)
            if (nbCols < 2)
                return 0;

            // Compute dimension
            this->_nbDim = nbCols;

            // Update successful
            return true;
        }
    }

    // Unknown or empty JSON, update failed
    return false;
}

bool VibesGraphicsArrow::computeProjection(int dimX, int dimY)
{
    const QJsonObject & json = this->_json;

    // Get shape color (or default if not specified)
    const QBrush & brush = vibesDefaults.brush(jsonValue("FaceColor").toString());
    const QPen & pen = vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString());

    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    Q_ASSERT(json.contains("type"));
    // VibesGraphicsArrow has JSON type "arrow"
    Q_ASSERT(json["type"].toString() == "arrow");
    // "bounds" is a matrix
    Q_ASSERT(isJsonMatrix(json["points"]));
    Q_ASSERT(json["tip_length"].toDouble() > 0);

    double before_last_x = 0., before_last_y = 0., last_x = 0., last_y = 0.;

    
    // update child items if they exist
    if (this->childItems().size() > 0)
    {
        // item type : 2 if path, 5 if polygon
        foreach(QGraphicsItem * item, this->childItems())
        {
            if (item->type() == 2)
            {
                //to vibes graphics item
                QGraphicsPathItem *graphics_path = qgraphicsitem_cast<QGraphicsPathItem *>(item);
                graphics_path->setPen(pen);
            }
            else if (item->type() == 5)
            {
                //to vibes graphics item
                QGraphicsPolygonItem *graphics_tip = qgraphicsitem_cast<QGraphicsPolygonItem *>(item);
                graphics_tip->setPen(pen);
                graphics_tip->setBrush(brush);
            }
        }
    }
    else{
        // Body
        {
            QPolygonF line;

            // Update line with projected points

            foreach(const QJsonValue value, json["points"].toArray())
            {
                // Read coordinates and append them to the list of points
                const QJsonArray coords = value.toArray();
                before_last_x = last_x;
                before_last_y = last_y;
                last_x = coords[dimX].toDouble();
                last_y = coords[dimY].toDouble();
                line << QPointF(last_x, last_y);
            }

            QPainterPath path;
            path.addPolygon(line);
            QGraphicsPathItem *graphics_path = new QGraphicsPathItem(path);
            graphics_path->setPen(pen);
            this->addToGroup(graphics_path);
        }

        // Tip
        {
            QPolygonF tip;

            double tip_length = json["tip_length"].toDouble();
            double dx = (before_last_x - last_x);
            double dy = (before_last_y - last_y);
            double arrow_angle = atan2(dy, dx);
            double tip_angle = 160. * M_PI / 180.0;

            double x = last_x;
            double y = last_y;

            // tip (right)
            tip << QPointF(x, y);
            // (upper left)
            tip << QPointF(x - cos(tip_angle + arrow_angle) * tip_length,
                        y - sin(tip_angle + arrow_angle) * tip_length);
            // (left)
            tip << QPointF(x + cos(arrow_angle) * tip_length * 2. / 3.,
                        y + sin(arrow_angle) * tip_length * 2. / 3.);
            // (bottom left)
            tip << QPointF(x - cos(-tip_angle + arrow_angle) * tip_length,
                        y - sin(-tip_angle + arrow_angle) * tip_length);

            QGraphicsPolygonItem *graphics_tip = new QGraphicsPolygonItem(tip);
            graphics_tip->setPen(pen);
            graphics_tip->setBrush(brush);
            this->addToGroup(graphics_tip);
        }
    }

    // Update successful
    return true;
}


//
// VibesGraphicsPie
//

bool VibesGraphicsPie::parseJsonGraphics(const QJsonObject &json)
{
    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    if (json.contains("type"))
    {
        // Retrieve type
        QString type = json["type"].toString();

        // VibesGraphicsPie has JSON type "arrow"
        if (type == "pie" && json.contains("center"))
        {
            QJsonArray center = json["center"].toArray();
            if (center.size() != 2) return false;
            if (json.contains("rho"))
            {
                if (json["rho"].toArray().size() != 2) return false;
            }
            else if (json.contains("theta"))
            {
                if (json["theta"].toArray().size() != 2) return false;
            }
            // Compute dimension
            this->_nbDim = center.size();

            // Update successful
            return true;
        }
    }

    // Unknown or empty JSON, update failed
    return false;
}

bool VibesGraphicsPie::computeProjection(int dimX, int dimY)
{
    const QJsonObject & json = this->_json;

    // Get shape color (or default if not specified)
    const QBrush & brush = vibesDefaults.brush(jsonValue("FaceColor").toString());
    const QPen & pen = vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString());

    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    Q_ASSERT(json.contains("type"));
    // VibesGraphicsPie has JSON type "pie"
    Q_ASSERT(json["type"].toString() == "pie");

    QJsonArray center = json["center"].toArray();
    QJsonArray rho = json["rho"].toArray();
    QJsonArray theta = json["theta"].toArray();

    Q_ASSERT(rho[0].toDouble() >= 0);
    Q_ASSERT(rho[1].toDouble() >= rho[0].toDouble());

    if (this->childItems().size() > 0)
    {
        foreach(QGraphicsItem * item, this->childItems())
        {
            //to vibes graphics item
            QGraphicsPathItem *graphics_polygon = qgraphicsitem_cast<QGraphicsPathItem *>(item);
            graphics_polygon->setPen(pen);
            graphics_polygon->setBrush(brush);
        }
    }
    else{
        // Body
        {

            double cx = center[0].toDouble();
            double cy = center[1].toDouble();

            double rho_m = rho[0].toDouble();
            double rho_p = rho[1].toDouble();
            double theta_m = -theta[0].toDouble();
            double theta_p = -theta[1].toDouble();

            // Angles are in degree and in clock-wise
            double m1_x = cx + rho_m * std::cos(-theta_m * M_PI / 180.0);
            double m1_y = cy + rho_m * std::sin(-theta_m * M_PI / 180.0);

            double m4_x = cx + rho_p * std::cos(-theta_m * M_PI / 180.0);
            double m4_y = cy + rho_p * std::sin(-theta_m * M_PI / 180.0);

            double dtheta = theta_p - theta_m;

            QPainterPath path(QPointF(m1_x, m1_y));
            path.lineTo(m4_x, m4_y);
            path.arcTo(QRectF(QPointF(cx - rho_p, cy - rho_p), QPointF(cx + rho_p, cy + rho_p)), theta_m, dtheta);
            path.arcTo(QRectF(QPointF(cx - rho_m, cy - rho_m), QPointF(cx + rho_m, cy + rho_m)), theta_p, -dtheta);

            // Draw with the new properties
            QGraphicsPathItem *graphics_path = new QGraphicsPathItem(path);
            graphics_path->setPen(pen);
            graphics_path->setBrush(brush);
            this->addToGroup(graphics_path);

        }
    }

    // Update successful
    return true;
}

bool VibesGraphicsPoint::parseJsonGraphics(const QJsonObject& json)
{
    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    if (json.contains("type"))
    {
        // Retrieve type
        QString type = json["type"].toString();
        // VibesGraphicsPoint has JSON type "point"
        if (type == "point" && json.contains("point"))
        {
            QJsonArray point = json["point"].toArray();
            if (point.size() != 2) return false;

            // Compute dimension
            this->_nbDim = point.size();

            this->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
            if (json.contains("FixedScale"))
            {
                // Default behaviour for points is true
                this->setFlag(QGraphicsItem::ItemIgnoresTransformations, json["FixedScale"].toBool(true));
            }
            if (json.contains("Draggable"))
            {
                if (json["Draggable"].isBool())
                {
                    this->setFlag(QGraphicsItem::ItemIsMovable, json["Draggable"].toBool(false));
                }
                else if (json["Draggable"].isDouble())
                {
                    this->setFlag(QGraphicsItem::ItemIsMovable, ((int) json["Draggable"].toDouble(0)) == 1);
                }
            }
            // Update successful
            return true;
        }
    }

    // Unknown or empty JSON, update failed
    return false;
}

bool VibesGraphicsPoint::computeProjection(int dimX, int dimY)
{
    const QJsonObject & json = this->_json;

    // Get arrow color (or default if not specified)


    // VibesGraphicsPie has JSON type "arrow"
    Q_ASSERT(json["type"].toString() == "point");
    // "bounds" is a matrix
    QJsonArray point = json["point"].toArray();

    // Body
    {
        double cx = point[0].toDouble();
        double cy = point[1].toDouble();

        const QBrush & brush = vibesDefaults.brush(jsonValue("FaceColor").toString());
        const QPen & pen = vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString());

        // Now process shape-specific properties
        // (we can only update properties of a shape, but mutation into another type is not supported)

        double rad = 0.01;
        if (json.contains("Radius"))
        {
            rad = json["Radius"].toDouble(0.01);
        }

        this->setRect(0, 0, 2 * rad, 2 * rad);

        this->setPos(cx, cy);

        this->setPen(pen);
        this->setBrush(brush);
    }

    // Update successful
    return true;
}

//
// VibesGraphicsPoints
//

bool VibesGraphicsPoints::parseJsonGraphics(const QJsonObject& json)
{
    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    if (json.contains("type"))
    {
        // Retrieve type
        QString type = json["type"].toString();

        // VibesGraphicsPie has JSON type "arrow"
        if (type == "points")
        {
            QJsonArray centers = json["centers"].toArray();
            this->_nbDim=centers[0].toArray().size();

            if (json.contains("Draggable"))
            {
                if (json["Draggable"].isBool())
                {
                    this->setFlag(QGraphicsItem::ItemIsMovable, json["Draggable"].toBool(false));
                }
                else if (json["Draggable"].isDouble())
                {
                    this->setFlag(QGraphicsItem::ItemIsMovable, ((int) json["Draggable"].toDouble(0)) == 1);
                }
            }
            QList<QGraphicsItem*> pts = this->childItems();

            foreach(QGraphicsItem* item, pts)
            {
                QGraphicsEllipseItem * disk = qgraphicsitem_cast<QGraphicsEllipseItem*>(item);
                if (!disk) continue;
                disk->setPen(vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString()));
                disk->setBrush(vibesDefaults.brush(jsonValue("FaceColor").toString()));
            }
            // Update successful
            return true;
        }
    }

    // Unknown or empty JSON, update failed
    return false;
}

bool VibesGraphicsPoints::computeProjection(int dimX, int dimY)
{
    const QJsonObject & json = this->_json;

    // Get shape color (or default if not specified)
    const QBrush & brush = vibesDefaults.brush(jsonValue("FaceColor").toString());
    const QPen & pen = vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString());

    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    Q_ASSERT(json.contains("type"));
    // VibesGraphicsRing has JSON type "ring"
    Q_ASSERT(json["type"].toString() == "points");

    // Before update, we first remove all existing points
    {
        QList<QGraphicsItem*> disks = this->childItems();
        foreach(QGraphicsItem* disk, disks) delete disk;
    }

    QJsonArray levels, radiuses;
    double radius = 0.01;
    bool levelsExist = false;//json.contains("ColorLevels");
    if (levelsExist)levels = json["ColorLevels"].toArray();
    bool radiusesExist = json.contains("Radiuses");
    if (radiusesExist)radiuses = json["Radiuses"].toArray();
    if (!radiusesExist)
    {
        if (json.contains("Radius"))
        {
            radius = json["Radius"].toDouble(0.01);
        }
    }
    QJsonArray centers = json["centers"].toArray();

    for (unsigned int i = 0; i < centers.size(); i++)
    {
        const QJsonArray point = centers[i].toArray();
        double x = point[dimX].toDouble();
        double y = point[dimY].toDouble();

        double r = radiusesExist ? radiuses[i].toDouble() : radius;

        // Draw with the new properties
        QGraphicsEllipseItem * disk = new QGraphicsEllipseItem(0, 0, 2 * r, 2 * r);
        disk->setPos(x, y);

        disk->setPen(pen);
        disk->setBrush(brush);

        disk->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
        if (json.contains("FixedScale"))
        {
            // Default behaviour for points is true
            disk->setFlag(QGraphicsItem::ItemIgnoresTransformations, json["FixedScale"].toBool(true));
        }
        this->addToGroup(disk);

        //if(levelsExist)
        //{
        // TODO: apply colormap
        //}
    }

    // Update successful
    return true;
}

//
// VibesGraphicsRing
//

bool VibesGraphicsRing::parseJsonGraphics(const QJsonObject& json)
{
    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    if (json.contains("type"))
    {
        // Retrieve type
        QString type = json["type"].toString();

        // VibesGraphicsPie has JSON type "arrow"
        if (type == "ring" && json.contains("center") && json.contains("rho"))
        {
            QJsonArray center = json["center"].toArray();
            if (center.size() != 2) return false;
            if (json["rho"].toArray().size() != 2) return false;
            // Compute dimension
            this->_nbDim = center.size();

            // Update successful
            return true;
        }
    }

    // Unknown or empty JSON, update failed
    return false;
}

bool VibesGraphicsRing::computeProjection(int dimX, int dimY)
{
    const QJsonObject & json = this->_json;

    // Get shape color (or default if not specified)
    const QBrush & brush = vibesDefaults.brush(jsonValue("FaceColor").toString());
    const QPen & pen = vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString());

    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    Q_ASSERT(json.contains("type"));
    // VibesGraphicsRing has JSON type "ring"
    Q_ASSERT(json["type"].toString() == "ring");

    QJsonArray center = json["center"].toArray();
    QJsonArray rho = json["rho"].toArray();

    Q_ASSERT(rho[0].toDouble() >= 0);
    Q_ASSERT(rho[1].toDouble() >= rho[0].toDouble());
    
    //
    // Body
    {
        double cx = center[0].toDouble();
        double cy = center[1].toDouble();

        double rho_m = rho[0].toDouble();
        double rho_p = rho[1].toDouble();

        QRectF boundingBoxP(cx - rho_p, cy - rho_p, 2 * rho_p, 2 * rho_p);

        QPainterPath path;
        path.addEllipse(boundingBoxP);

        QRectF boundingBoxM(cx - rho_m, cy - rho_m, 2 * rho_m, 2 * rho_m);
        QPainterPath pathM;
        pathM.addEllipse(boundingBoxM);

        path = path.subtracted(pathM);


        // Draw with the new properties
        QGraphicsPathItem *graphics_path = new QGraphicsPathItem(path);
        graphics_path->setPen(pen);
        graphics_path->setBrush(brush);
        this->addToGroup(graphics_path);
    }

    // Update successful
    return true;
}

bool VibesGraphicsRaster::parseJsonGraphics(const QJsonObject& json)
{
    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    if (json.contains("type"))
    {
        // Retrieve type
        QString type = json["type"].toString();

        // VibesGraphicsPie has JSON type "arrow"
        if (type == "raster" && json.contains("filename") && json.contains("ul_corner") && json.contains("scale"))
        {
            if (json["ul_corner"].toArray().size() != 2) return false;
            if (json["scale"].toArray().size() != 2) return false;
            // Compute dimension
            this->_nbDim = 2; //center.size();
            // Update successful
            return true;
        }
    }

    // Unknown or empty JSON, update failed
    return false;
}
#include <iostream>
bool VibesGraphicsRaster::computeProjection(int dimX, int dimY)
{
    const QJsonObject & json = this->_json;

    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    Q_ASSERT(json.contains("type"));
    // VibesGraphicsRing has JSON type "ring"
    Q_ASSERT(json["type"].toString() == "raster");


    // Body
    {
        QString filename = json["filename"].toString();
        QJsonArray ul_corner = json["ul_corner"].toArray();
        QJsonArray scale = json["scale"].toArray();
        double xlb = ul_corner[0].toDouble();
        double yub = ul_corner[1].toDouble();
        double xres = scale[0].toDouble();
        double yres = scale[1].toDouble();

        QImage image(filename);
        QPixmap pixmap = QPixmap::fromImage(image);
        if (json.contains("EdgeColor")) {
          const QPen & pen = vibesDefaults.pen(jsonValue("EdgeColor").toString(),jsonValue("LineStyle").toString(),jsonValue("LineWidth").toString());
          pixmap.setMask(pixmap.createMaskFromColor(pen.color().rgb()));

        }

        QTransform transform(xres, 0, 0, yres, xlb, yub);
        QGraphicsPixmapItem *pixmap_item = new QGraphicsPixmapItem(pixmap);

        pixmap_item->setShapeMode(QGraphicsPixmapItem::MaskShape);
        pixmap_item->setTransform(transform);

        this->addToGroup(pixmap_item);
    }

    // Update successful
    return true;
}
