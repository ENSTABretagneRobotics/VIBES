#ifndef VIBESSCENE2D_H
#define VIBESSCENE2D_H

#include <QGraphicsScene>
#include <QHash>
class QJsonObject;
class VibesGraphicsItem;

class VibesScene2D : public QGraphicsScene
{
    Q_OBJECT

    int _dimX, _dimY;
    int _nbDim;
    QHash<QString,VibesGraphicsItem *> _namedItems;
    QHash<int, QString> _dimNames;
public:
    explicit VibesScene2D(QObject *parent = 0);
    ~VibesScene2D();
    VibesGraphicsItem *addJsonShapeItem(const QJsonObject &shape);

    void addVibesItem(VibesGraphicsItem *item);
    VibesGraphicsItem * itemByName(QString name) {if (_namedItems.contains(name)) return _namedItems[name]; else return 0;}
    void setItemName(VibesGraphicsItem *item, QString name);
    QList<QString> namedItems() const {return _namedItems.keys();}

    const int nbDim() const { return _nbDim; }
    const int dimX() const { return _dimX; }
    const int dimY() const { return _dimY; }

    bool setDims(int dimX, int dimY);

    QString dimName(int dim) { if (dim<0 || dim>=nbDim()) return QString();
                               else if (_dimNames.contains(dim)) return _dimNames[dim];
                               else return QString("dim %1").arg(dim); }
    void setDimName(int dim, QString name) { if (dim>=0 && dim<nbDim()) { _dimNames[dim]=name; emit dimensionsChanged(); } }
signals:
    void changedDimX(int);
    void changedDimY(int);
    void dimensionsChanged();

public slots:
    bool setDimX(int dimX);
    bool setDimY(int dimY);

private:
    void updateDims();
};

#endif // VIBESSCENE2D_H
