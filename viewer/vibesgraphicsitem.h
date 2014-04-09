#ifndef VIBESGRAPHICSITEM_H
#define VIBESGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QJsonObject>
#include <QBitArray>

// VibesDefaults includes
#include <QHash>
#include <QPen>

// Singleton class to hold Vibes defaults and constants
class VibesDefaults {
    QHash<QString, QBrush> _brushes;
    QPen _pen;
public:
    static const VibesDefaults & instance() { return _instance; }
    const QBrush brush(const QString & name = QString()) const { return _brushes[name]; }
    const QPen pen() const { return _pen; }
private:
    VibesDefaults();
    static VibesDefaults _instance;
    void initDefaultBrushes();
};
// Helper macro to access the VibesDefaults instance
#define vibesDefaults VibesDefaults::instance()

class VibesGraphicsItem
{
public:
    enum { VibesGraphicsItemType = QGraphicsItem::UserType,
           VibesGraphicsBoxType,
           VibesGraphicsBoxesType,
           VibesGraphicsEllipseType,
           VibesGraphicsLastType // Do not remove! Signals the end of VibesGraphicsItem types
         };
    VibesGraphicsItem();
    bool setJson(const QJsonObject &json, int dimX=0, int dimY=1);
    const QJsonObject & json() const { return _json; }
    bool existsInProj(int dimX, int dimY) const { return hasDim(dimX) && hasDim(dimY); }
    bool setProj(int dimX, int dimY);
    int dimension() const { return maxDim(); }
protected:
    virtual bool parseJson(const QJsonObject &json) = 0;
    virtual bool computeProjection(int dimX=0, int dimY=1) = 0;
    virtual bool hasDim(int n) const { return n>=0 && n<_nbDim; }
    virtual int maxDim() const { return _nbDim; }
protected:
    QJsonObject _json;
    int _nbDim;
};


inline VibesGraphicsItem * vibesgraphicsitem_cast(QGraphicsItem *item)
{
    return (item && item->type() >= VibesGraphicsItem::VibesGraphicsItemType
            && item->type() < VibesGraphicsItem::VibesGraphicsLastType) ? dynamic_cast<VibesGraphicsItem*>(item) : 0;
}

inline const VibesGraphicsItem * vibesgraphicsitem_cast(const QGraphicsItem *item)
{
    return (item && item->type() >= VibesGraphicsItem::VibesGraphicsItemType
            && item->type() < VibesGraphicsItem::VibesGraphicsLastType) ? dynamic_cast<const VibesGraphicsItem*>(item) : 0;
}


// A macro for defining the type method of VibesGraphicsItem subclasses
#define VIBES_GRAPHICS_ITEM_TYPE_DECL(class_name) \
    enum { Type = class_name ## Type }; \
    int type() const { return Type; }
// A macro for defining the constructor of VibesGraphicsItem subclasses
#define VIBES_GRAPHICS_ITEM_CTOR_DECL(class_name, base_class) \
    class_name(QGraphicsItem * parent = 0) : base_class(parent), VibesGraphicsItem() {} \
    class_name(const QJsonObject &json, QGraphicsItem * parent = 0) : base_class(parent), VibesGraphicsItem() { setJson(json); }
// A macro for defining VibesGraphicsItem subclasses
#define VIBES_GRAPHICS_ITEM(class_name, base_class) \
public: \
    VIBES_GRAPHICS_ITEM_TYPE_DECL(class_name) \
    VIBES_GRAPHICS_ITEM_CTOR_DECL(class_name, base_class)


/// A box

class VibesGraphicsBox : public QGraphicsRectItem, public VibesGraphicsItem
{
    VIBES_GRAPHICS_ITEM(VibesGraphicsBox, QGraphicsRectItem)
protected:
    bool parseJson(const QJsonObject &json);
    bool computeProjection(int dimX, int dimY);
};

/// A set of boxes

class VibesGraphicsBoxes : public QGraphicsPathItem, public VibesGraphicsItem
{
    VIBES_GRAPHICS_ITEM(VibesGraphicsBoxes, QGraphicsPathItem)
protected:
    bool parseJson(const QJsonObject &json);
    bool computeProjection(int dimX, int dimY);
};


/// An ellipse or confidence ellipse

class VibesGraphicsEllipse : public QGraphicsEllipseItem, public VibesGraphicsItem
{
    VIBES_GRAPHICS_ITEM(VibesGraphicsEllipse, QGraphicsEllipseItem)
protected:
    bool parseJson(const QJsonObject &json);
    bool computeProjection(int dimX, int dimY);
    void axisAngleFromCovarianceK(const double &sxx, const double &syy, const double &sxy, const double &k,
                                  double &wx, double &wy, double &angle);
};

#endif // VIBESGRAPHICSITEM_H
