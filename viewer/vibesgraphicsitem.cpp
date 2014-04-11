#include "vibesgraphicsitem.h"

#include <QtCore>
#include <QVector>

#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsItemGroup>
#include <QGraphicsPathItem>
#include <QPainterPath>


#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

// The only instance of VibesDefaults
VibesDefaults VibesDefaults::_instance;

VibesDefaults::VibesDefaults()
 : _pen(Qt::black, 0)
{
    initDefaultBrushes();
}

/// Initializes brushes for default color names

void VibesDefaults::initDefaultBrushes()
{
#define ADD_DEFAULT_BRUSH(full_name) \
    _brushes[ #full_name ]  = QBrush(Qt::full_name);

#define ADD_DEFAULT_BRUSH2(full_name, short_name) \
    _brushes[ #full_name ]  = QBrush(Qt::full_name); \
    _brushes[ #short_name ] = QBrush(Qt::full_name);

    // Default brush
    _brushes[QString()] = QBrush();

    // Named brushes
    ADD_DEFAULT_BRUSH2(transparent, none);

    ADD_DEFAULT_BRUSH2(cyan, c);
    ADD_DEFAULT_BRUSH2(yellow, y);
    ADD_DEFAULT_BRUSH2(magenta, m);
    ADD_DEFAULT_BRUSH2(red, r);
    ADD_DEFAULT_BRUSH2(green, g);
    ADD_DEFAULT_BRUSH2(blue, b);
    ADD_DEFAULT_BRUSH2(black, k);
    ADD_DEFAULT_BRUSH2(white, w);
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



VibesGraphicsItem::VibesGraphicsItem(QGraphicsItem *qGraphicsItem)
 : _qGraphicsItem(qGraphicsItem), _nbDim(0)
{
}

bool VibesGraphicsItem::setJson(const QJsonObject &json, int dimX, int dimY)
{
    if (!parseJson(json)) return false;
    _json = json;

    setProj(dimX, dimY);
    return true;
}

bool VibesGraphicsItem::setProj(int dimX, int dimY)
{
    if (existsInProj(dimX,dimY))
    {
        return computeProjection(dimX, dimY);
    }
    else
    {
        /// \todo Change visibility to none when projection is not available
        return false;
    }
}

VibesGraphicsItem * VibesGraphicsItem::newWithType(const QString type)
{
    if (type == "box") {
        return new VibesGraphicsBox();
    }
    else if (type == "boxes") {
        return new VibesGraphicsBoxes();
    }
    else if (type == "boxes union") {
        return new VibesGraphicsBoxesUnion();
    }
    else if (type == "ellipse") {
        return new VibesGraphicsEllipse();
    }
    else if (type == "point") {
        //! \todo Implement "point" type
    }
    else if (type == "points") {
        //! \todo Implement "points" type
    }
    else if (type == "line") {
        return new VibesGraphicsLine();
    }
    return 0;
}


bool VibesGraphicsItem::isJsonMatrix(const QJsonValue value, int &nbRows, int &nbCols)
{
    nbRows = 0; nbCols = 0;
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


bool VibesGraphicsBox::parseJson(const QJsonObject &json)
{
    // Get shape color (or default if not specified)
    const QBrush & brush = vibesDefaults.brush( json.value("color").toString() );
    const QPen & pen = vibesDefaults.pen();

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
            if (bounds.size() < 4 || bounds.size()%2 != 0)
                return false;

            // Compute dimension
            this->_nbDim = bounds.size() / 2;

            // Update rectangle color
            this->setPen(pen);
            this->setBrush(brush);

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

    Q_ASSERT(json.contains("type"));
    Q_ASSERT(json["type"].toString() == "box");

    QJsonArray bounds = json["bounds"].toArray();
    // Compute dimension
    Q_ASSERT(this->_nbDim == bounds.size() / 2);
    Q_ASSERT(bounds.size() >= (2*(qMax(dimX,dimY)+1)));

    // Read bounds
    double lb_x = bounds[2*dimX].toDouble();
    double ub_x = bounds[2*dimX+1].toDouble();
    double lb_y = bounds[2*dimY].toDouble();
    double ub_y = bounds[2*dimY+1].toDouble();

    // Update rectangle
    this->setRect(lb_x, lb_y, ub_x - lb_x, ub_y - lb_y);

    // Update successful
    return true;
}

//
// VibesGraphicsBoxes
//

bool VibesGraphicsBoxes::parseJson(const QJsonObject &json)
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
            if (nbCols%2 != 0)
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
    const QBrush & brush = vibesDefaults.brush( json.value("color").toString() );
    const QPen & pen = vibesDefaults.pen();

    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    Q_ASSERT (json.contains("type"));
    // VibesGraphicsBoxes has JSON type "boxes"
    Q_ASSERT ( json["type"].toString() == "boxes" );

    // VibesGraphicsBoxes is basically a QGraphicsItemGroup containing QGraphicsRectItems
    // Before update, we first remove all existing rectangles
    {
        QList<QGraphicsItem*> rects = this->childItems();
        foreach(QGraphicsItem* rect, rects) delete rect;
    }
    // "bounds" is a matrix
    Q_ASSERT ( isJsonMatrix(json["bounds"]) );

    // Fill group with projected boxes
    foreach (const QJsonValue value, json["bounds"].toArray()) {
        const QJsonArray box = value.toArray();
        // Read bounds
        double lb_x = box[2*dimX].toDouble();
        double ub_x = box[2*dimX+1].toDouble();
        double lb_y = box[2*dimY].toDouble();
        double ub_y = box[2*dimY+1].toDouble();
        // Create a new rect
        QGraphicsRectItem * rect = new QGraphicsRectItem(lb_x, lb_y, ub_x - lb_x, ub_y - lb_y);
        rect->setPen(pen);
        rect->setBrush(brush);
        // Add it to the group
        this->addToGroup( rect );
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

bool VibesGraphicsBoxesUnion::parseJson(const QJsonObject &json)
{
    // Get shape color (or default if not specified)
    const QBrush & brush = vibesDefaults.brush( json.value("color").toString() );
    const QPen & pen = vibesDefaults.pen();

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
            if (nbCols%2 != 0)
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
            // Set graphics properties
            this->setPen(pen);
            this->setBrush(brush);

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

    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    Q_ASSERT (json.contains("type"));
    // VibesGraphicsBoxes has JSON type "boxes union"
    Q_ASSERT ( json["type"].toString() == "boxes union" );
    // "bounds" is a matrix
    Q_ASSERT ( isJsonMatrix(json["bounds"]) );

    // Update path with projected boxes
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    foreach (const QJsonValue value, json["bounds"].toArray()) {
        const QJsonArray box = value.toArray();
        // Read bounds
        double lb_x = box[2*dimX].toDouble();
        double ub_x = box[2*dimX+1].toDouble();
        double lb_y = box[2*dimY].toDouble();
        double ub_y = box[2*dimY+1].toDouble();
        // Create a new rect path
        QPainterPath rect_path;
        rect_path.addRect(lb_x, lb_y, ub_x - lb_x, ub_y - lb_y);
        // Unite with current path
        path |= rect_path;
    }
    this->setPath(path);

    // Update successful
    return true;
}


//
// VibesGraphicsEllipse
//

bool VibesGraphicsEllipse::parseJson(const QJsonObject &json)
{
    // Get shape color (or default if not specified)
    const QBrush & brush = vibesDefaults.brush( json.value("color").toString() );
    const QPen & pen = vibesDefaults.pen();

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
                else if (json.contains("covariance"))
                {
                    // A covariance matrix has been provided (Confidence ellipse)
                    double k = json.contains("sigma") ? json["sigma"].toDouble() : 5;
                    QJsonArray covariance = json["covariance"].toArray();
                    if (covariance.size() != pow(center.size(),2)) return false;
                }
                else
                {
                    // Should not be here, ellipse parameters have not been provided
                    return false;
                }
                // Set dimension
                this->_nbDim = center.size();
                // Update ellipse properties
                this->setPen(pen);
                this->setBrush(brush);
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

    Q_ASSERT (json.contains("type"));
    Q_ASSERT(json["type"].toString() == "ellipse");

    QJsonArray center = json["center"].toArray();

    Q_ASSERT (center.size() >= 2);
    Q_ASSERT(this->_nbDim == center.size());
    Q_ASSERT(center.size() >= (qMax(dimX,dimY)+1));

    // Get center
    double x = center[dimX].toDouble();
    double y = center[dimY].toDouble();

    // Semi-major and semi-minor axes, and rotation
    double wx, wy, angle;

    Q_ASSERT((json.contains("axis") && json.contains("orientation")) || json.contains("covariance"));
    if (json.contains("axis") && json.contains("orientation"))
    {
        // Semi-major and semi-minor axes are directly provided (Ellipse or circle)
        Q_ASSERT ((dimX==0 && dimY==1) || (dimX==1 && dimY==0));

        QJsonArray axis = json["axis"].toArray();
        wx = axis[0].toDouble();
        wy = axis[1].toDouble();
        if (dimX==0 && dimY==1)
            angle = json.value("orientation").toDouble();
        else if (dimX==1 && dimY==0)
            angle = 90.0 - json.value("orientation").toDouble();
        else
            return false;
    }
    else if (json.contains("covariance"))
    {
        // A covariance matrix has been provided (Confidence ellipse)
        double k = json.contains("sigma") ? json["sigma"].toDouble() : 5;
        QJsonArray covariance = json["covariance"].toArray();
        Q_ASSERT (covariance.size() == center.size()*center.size());
        double sxx = covariance[dimX + center.size()*dimX].toDouble();
        double sxy = covariance[dimX + center.size()*dimY].toDouble();
        double syy = covariance[dimY + center.size()*dimY].toDouble();
        // Compute the semi-major and semi-minor axes and rotation angle.
        axisAngleFromCovarianceK(sxx,syy,sxy,k, wx,wy,angle);
    }
    else
    {
        // Should not be here, ellipse parameters have not been provided
        return false;
    }
    // Update ellipse
    this->setRect(-wx, -wy, 2 * wx, 2 * wy);
    this->setRotation(angle);
    this->setPos(x, y);
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

bool VibesGraphicsLine::parseJson(const QJsonObject &json)
{
    // Get shape color (or default if not specified)
   // const QBrush & brush = vibesDefaults.brush( json.value("color").toString() );
    const QPen & pen = vibesDefaults.pen( json.value("color").toString() );

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
            // Set graphics properties
            this->setPen(pen);
            //this->setBrush(brush);

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

    // Now process shape-specific properties
    // (we can only update properties of a shape, but mutation into another type is not supported)
    Q_ASSERT (json.contains("type"));
    // VibesGraphicsLine has JSON type "line"
    Q_ASSERT ( json["type"].toString() == "line" );
    // "bounds" is a matrix
    Q_ASSERT ( isJsonMatrix(json["points"]) );

    // Update line with projected points
    QPainterPath path;
    QPolygonF polygon;
    foreach (const QJsonValue value, json["points"].toArray()) {
        // Read coordinates and append them to the list of points
        const QJsonArray coords = value.toArray();
        polygon << QPointF(coords[dimX].toDouble(), coords[dimY].toDouble());
    }
    // Update polygon with the list of vertices
    path.addPolygon(polygon);
    this->setPath(path);

    // Update successful
    return true;
}
