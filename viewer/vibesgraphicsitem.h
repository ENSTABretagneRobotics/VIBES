#ifndef VIBESGRAPHICSITEM_H
#define VIBESGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

//#include <QBitArray>
#include "vibesscene2d.h"

// VibesDefaults includes
#include <QHash>
#include <QPen>

#// Singleton class to hold Vibes defaults and constants
class VibesDefaults {
    QHash<QString, QBrush> _brushes;
    QHash<QString, QPen> _pens;
public:
    static VibesDefaults & instance() { return _instance; }
    
    static QString rgbaToString(const QJsonArray &array, double &r, double &g, double &b, double &a)
    {
        bool ok;
        if(array.size()>=1)
        {
            if(array[0].isString())
            {
                r=array[0].toString().toDouble(&ok);
                r=ok?r:0;
            }
            else if(array[0].isDouble())
            {
                r=array[0].toDouble(0);
            }
        }
        
        
        if(array.size()>=2)
        {
            if(array[1].isString())
            {
                g=array[1].toString().toDouble(&ok);
                g=ok?g:0;
            }
            else if(array[1].isDouble())
            {
                g=array[1].toDouble(0);
            }
        }
        
        if(array.size()>=3)
        {
            if(array[2].isString())
            {
                b=array[2].toString().toDouble(&ok);
                b=ok?b:0;
            }
            else if(array[2].isDouble())
            {
                r=array[2].toDouble(0);
            }
        }
        
        if(array.size()>=4)
        {
            if(array[3].isString())
            {
                a=array[3].toString().toDouble(&ok);
                a=ok?a:255;
            }
            else if(array[3].isDouble())
            {
                a=array[3].toDouble(255);
            }
        }
        return QString("%1,%2,%3,%4").arg(r).arg(g).arg(b).arg(a);
    }
    
    static Qt::PenStyle styleToPenStyle(const std::string &style)
    {
        if(style=="--")
        {
            return Qt::DashLine;
        }
        if(style=="-.")
        {
            return Qt::DashDotLine;
        }
        if(style==":")
        {
            return Qt::DotLine;
        }
        if(style=="-..")
        {
            return Qt::DashDotDotLine;
        }
        return Qt::SolidLine;
    }
    
    const QBrush brush(const QJsonValue &value) { 
        QString name;
        if(value.isArray())
        {
            double r=0,g=0,b=0,a=255;
            
            name=VibesDefaults::rgbaToString(value.toArray(),r,g,b,a);
            if(!_brushes.contains(name))
            {
                QColor c(r,g,b,a);
                _brushes[name]=QBrush(c);
            }
        }
        else if(value.isString())
        {
        
            QString valString=value.toString();
            if( !_brushes.contains(valString))
            {
                _brushes[name] = QBrush(parseColorName(name));
            }
        }
        return _brushes[name]; 
    }
    
    const QPen pen(const QJsonValue &value) {
        QString name;
        if(value.isArray())
        {
            double r=0,g=0,b=0,a=255;
            
            const QString parsedName=VibesDefaults::rgbaToString(value.toArray(),r,g,b,a);
            name=parsedName;
            if(!_pens.contains(name))
            {
                _pens[parsedName]=QPen(QColor(r,g,b,a),0);
            }
        }
        else if(value.isString())
        {
            const QString parsedName=value.toString();
            name=parsedName;
            if(!_pens.contains(parsedName))
            {
                _pens[parsedName]=QPen(parseColorName(name),0);
            }
        }
        return _pens[name];
    }
    
    static const QColor parseColorName(const QString& name){
        if(name.contains(","))
        {
            QStringList sl=name.split(",");
            double r=0,g=0,b=0,a=255;
            rgbaToString(QJsonArray::fromStringList(sl),r,g,b,a);
            return QColor(r,g,b,a);
        }
        else if(name.startsWith("#")&&(name.size()==7||name.size()==9))
        {
            Q_ASSERT(name.size() == 7 || name.size() == 9);
            // Suported format #RRGGBB and #RRGGBBAA
            QColor color;
            color.setNamedColor(name.mid(0,7));
            // if len of name > 7 the 2 last caracters are the alpha value
            if(name.size() > 7)
            {
                color.setAlpha(name.mid(7,2).toUInt(0,16));
            }
            return color;
        }
        
        QColor c(name.toLower());
        //if(c.isValid())
        //    return c;
        return c;
    }
private:
    VibesDefaults();
    static VibesDefaults _instance;
    void initDefaultBrushesAndPens();
};
// Helper macro to access the VibesDefaults instance
#define vibesDefaults VibesDefaults::instance()

class VibesGraphicsItem
{
    // Pointer to the QGraphicsItem object. Used for casting.
    QGraphicsItem * _qGraphicsItem;
    QString _name;
    int _dimX, _dimY;
public:
    enum { VibesGraphicsItemType = QGraphicsItem::UserType,
           // Group of item
           VibesGraphicsGroupType,
           // Primitive types
           VibesGraphicsBoxType,
           VibesGraphicsEllipseType,
           VibesGraphicsPolygonType,
           VibesGraphicsArrowType,
           VibesGraphicsPieType,
           VibesGraphicsRingType,
           // Complex types based on primitive types
           VibesGraphicsVehicleType,
           VibesGraphicsVehicleAUVType,
           // List based types
           //VibesGraphicsPointsType,
           VibesGraphicsLineType,
           VibesGraphicsBoxesType,
           VibesGraphicsBoxesUnionType,
           // Do not remove the following value! It signals the end of VibesGraphicsItem types
           VibesGraphicsLastType
         };
    // Constructor
    VibesGraphicsItem(QGraphicsItem * qGraphicsItem);
    // Destructor (virtual)
    virtual ~VibesGraphicsItem() { setName(QString()); }

    bool setJson(QJsonObject json, int dimX, int dimY);
    bool setJson(QJsonObject json) { return setJson(json, _dimX, _dimY); }
    const QJsonObject & json() const { return _json; }
    QJsonValue jsonValue(const QString &key) const;
    void setJsonValue(const QString &key, const QJsonValue &value);
    void setJsonValues(const QJsonObject &values);

    bool existsInProj(int dimX, int dimY) const { return hasDim(dimX) && hasDim(dimY); }
    bool setProj(int dimX, int dimY);
    bool updateProj() { return setProj(_dimX,_dimY); }
    int dimension() const { return maxDim(); }

    QString name() const { return _name; }
    void setName(QString name) { if (name != this->name()) { _name=name; if (scene()) scene()->setItemName(this, this->name()); } }
    VibesScene2D* scene() const { if (_qGraphicsItem) return static_cast<VibesScene2D*>( _qGraphicsItem->scene() ); else return 0;}

    operator QGraphicsItem& () { Q_ASSERT(_qGraphicsItem!=0); return *_qGraphicsItem; }
    operator const QGraphicsItem& () const { Q_ASSERT(_qGraphicsItem!=0); return *_qGraphicsItem; }

    static VibesGraphicsItem *newWithType(const QString type);

protected:
    bool parseJson(QJsonObject &json);
    virtual bool parseJsonGraphics(const QJsonObject &json) = 0;
    virtual bool computeProjection(int dimX, int dimY) = 0;
    virtual bool hasDim(int n) const { return n>=0 && n<_nbDim; }
    virtual int maxDim() const { return _nbDim; }
    // Utility
    static bool isJsonMatrix(const QJsonValue json, int &nbRows, int &nbCols);
    static bool isJsonMatrix(const QJsonValue json) { int r,c; return isJsonMatrix(json, r, c); }
    // Json Properties categories
    virtual bool propertyIsReadOnly(const QString & key) { if (key=="type") return true; else return false; }
    virtual bool propertyChangesGeometry(const QString & key) { return false; }

protected:
    QJsonObject _json;
    int _nbDim;
};

// Specialization of qgraphicsitem_cast to VibesGraphicsItem* base class (uses dynamic_cast)
template<> inline VibesGraphicsItem * qgraphicsitem_cast(QGraphicsItem *item)
{
    return (item && item->type() >= VibesGraphicsItem::VibesGraphicsItemType
            && item->type() < VibesGraphicsItem::VibesGraphicsLastType) ? dynamic_cast<VibesGraphicsItem*>(item) : 0;
}

template<> inline const VibesGraphicsItem * qgraphicsitem_cast(const QGraphicsItem *item)
{
    return (item && item->type() >= VibesGraphicsItem::VibesGraphicsItemType
            && item->type() < VibesGraphicsItem::VibesGraphicsLastType) ? dynamic_cast<const VibesGraphicsItem*>(item) : 0;
}

// Provides a cast to GraphicsItem classes from VibesGraphicsItem base class, without using dynamic_cast.
template <class T> inline T vibesgraphicsitem_cast(VibesGraphicsItem *item)
{
    return item ? qgraphicsitem_cast<T>(&(static_cast<QGraphicsItem&>(*item))) : 0;
}

template <class T> inline const T vibesgraphicsitem_cast(const VibesGraphicsItem *item)
{
    return item ? qgraphicsitem_cast<T>(&(static_cast<const QGraphicsItem&>(*item))) : 0;
}


// A macro for defining the type method of VibesGraphicsItem subclasses
#define VIBES_GRAPHICS_ITEM_TYPE_DECL(class_name) \
    enum { Type = class_name ## Type }; \
    int type() const { return Type; }
// A macro for defining the constructor of VibesGraphicsItem subclasses
#define VIBES_GRAPHICS_ITEM_CTOR_DECL(class_name, base_class) \
    class_name(QGraphicsItem * parent = 0) : base_class(parent), VibesGraphicsItem(this) {} \
    class_name(const QJsonObject &json, QGraphicsItem * parent = 0) : base_class(parent), VibesGraphicsItem(this) { setJson(json); }
// A macro for defining VibesGraphicsItem subclasses
#define VIBES_GRAPHICS_ITEM(class_name, base_class) \
public: \
    VIBES_GRAPHICS_ITEM_TYPE_DECL(class_name) \
    VIBES_GRAPHICS_ITEM_CTOR_DECL(class_name, base_class)

#define VIBES_READ_ONLY_PROPERTIES(...) \
protected: \
inline bool propertyIsReadOnly(const QString& key) { \
    if (QStringList({__VA_ARGS__}).contains(key)) return true; \
    else return VibesGraphicsItem::propertyIsReadOnly(key); }
#define VIBES_GEOMETRY_CHANGING_PROPERTIES(...) \
protected: \
inline bool propertyChangesGeometry(const QString& key) { \
    if (QStringList({__VA_ARGS__}).contains(key)) return true; \
    else return VibesGraphicsItem::propertyChangesGeometry(key); }

/// A group of objects (a layer)

class VibesGraphicsGroup : public QGraphicsItemGroup, public VibesGraphicsItem
{
    VIBES_GRAPHICS_ITEM(VibesGraphicsGroup, QGraphicsItemGroup)
public:
    void addToGroup(VibesGraphicsItem *item);
    void clear();
protected:
    bool parseJsonGraphics(const QJsonObject &json);
    bool computeProjection(int dimX, int dimY) { return true; }
};

/// A box

class VibesGraphicsBox : public QGraphicsRectItem, public VibesGraphicsItem
{
    VIBES_GRAPHICS_ITEM(VibesGraphicsBox, QGraphicsRectItem)
    VIBES_GEOMETRY_CHANGING_PROPERTIES("bounds")
protected:
    bool parseJsonGraphics(const QJsonObject &json);
    bool computeProjection(int dimX, int dimY);
};

/// A set of boxes

class VibesGraphicsBoxes : public QGraphicsItemGroup, public VibesGraphicsItem
{
    VIBES_GRAPHICS_ITEM(VibesGraphicsBoxes, QGraphicsItemGroup)
    VIBES_GEOMETRY_CHANGING_PROPERTIES("bounds")
protected:
    bool parseJsonGraphics(const QJsonObject &json);
    bool computeProjection(int dimX, int dimY);
};

/// The union of a set of boxes

class VibesGraphicsBoxesUnion : public QGraphicsPathItem, public VibesGraphicsItem
{
    VIBES_GRAPHICS_ITEM(VibesGraphicsBoxesUnion, QGraphicsPathItem)
    VIBES_GEOMETRY_CHANGING_PROPERTIES("bounds")
protected:
    bool parseJsonGraphics(const QJsonObject &json);
    bool computeProjection(int dimX, int dimY);
};


/// An ellipse or confidence ellipse

class VibesGraphicsEllipse : public QGraphicsEllipseItem, public VibesGraphicsItem
{
    VIBES_GRAPHICS_ITEM(VibesGraphicsEllipse, QGraphicsEllipseItem)
    VIBES_GEOMETRY_CHANGING_PROPERTIES("center","axis","orientation","covariance","sigma")
protected:
    bool parseJsonGraphics(const QJsonObject &json);
    bool computeProjection(int dimX, int dimY);
    void axisAngleFromCovarianceK(const double &sxx, const double &syy, const double &sxy, const double &k,
                                  double &wx, double &wy, double &angle);
};


/// A line (defined by a set of points)

class VibesGraphicsLine : public QGraphicsPathItem, public VibesGraphicsItem
{
    VIBES_GRAPHICS_ITEM(VibesGraphicsLine, QGraphicsPathItem)
    VIBES_GEOMETRY_CHANGING_PROPERTIES("points")
protected:
    bool parseJsonGraphics(const QJsonObject &json);
    bool computeProjection(int dimX, int dimY);
};

/// A polygon

class VibesGraphicsPolygon : public QGraphicsPolygonItem, public VibesGraphicsItem
{
    VIBES_GRAPHICS_ITEM(VibesGraphicsPolygon, QGraphicsPolygonItem)
    VIBES_GEOMETRY_CHANGING_PROPERTIES("bounds")
protected:
    bool parseJsonGraphics(const QJsonObject &json);
    bool computeProjection(int dimX, int dimY);
};

/// A simple vehicle (triangle)

class VibesGraphicsVehicle : public QGraphicsItemGroup, public VibesGraphicsItem
{
    VIBES_GRAPHICS_ITEM(VibesGraphicsVehicle, QGraphicsItemGroup)
    VIBES_GEOMETRY_CHANGING_PROPERTIES("center","length","orientation")
protected:
    bool parseJsonGraphics(const QJsonObject &json);
    bool computeProjection(int dimX, int dimY);
};

/// A submarine vehicle type AUV (torpedo)

class VibesGraphicsVehicleAUV : public QGraphicsItemGroup, public VibesGraphicsItem
{
    VIBES_GRAPHICS_ITEM(VibesGraphicsVehicleAUV, QGraphicsItemGroup)
    VIBES_GEOMETRY_CHANGING_PROPERTIES("center","length","orientation")
protected:
    bool parseJsonGraphics(const QJsonObject &json);
    bool computeProjection(int dimX, int dimY);
};

/// An arrow

class VibesGraphicsArrow : public QGraphicsItemGroup, public VibesGraphicsItem
{
    VIBES_GRAPHICS_ITEM(VibesGraphicsArrow, QGraphicsItemGroup)
    VIBES_GEOMETRY_CHANGING_PROPERTIES("points","tip_length")
protected:
    bool parseJsonGraphics(const QJsonObject &json);
    bool computeProjection(int dimX, int dimY);
};

/// A Pie

class VibesGraphicsPie : public QGraphicsItemGroup, public VibesGraphicsItem
{
    VIBES_GRAPHICS_ITEM(VibesGraphicsPie, QGraphicsItemGroup)
    VIBES_GEOMETRY_CHANGING_PROPERTIES("center","rho", "theta")
protected:
    bool parseJsonGraphics(const QJsonObject &json);
    bool computeProjection(int dimX, int dimY);
};

class VibesGraphicsRing : public QGraphicsItemGroup, public VibesGraphicsItem
{
    VIBES_GRAPHICS_ITEM(VibesGraphicsRing, QGraphicsItemGroup)
    VIBES_GEOMETRY_CHANGING_PROPERTIES("center","rho")
protected:
    bool parseJsonGraphics(const QJsonObject &json);
    bool computeProjection(int dimX, int dimY);
};

#endif // VIBESGRAPHICSITEM_H
