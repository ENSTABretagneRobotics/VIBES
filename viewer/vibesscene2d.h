#ifndef VIBESSCENE2D_H
#define VIBESSCENE2D_H

#include <QGraphicsScene>
#include <QJsonObject>

class VibesScene2D : public QGraphicsScene
{
    Q_OBJECT

    int _dimX, _dimY;
    int _nbDim;
public:
    explicit VibesScene2D(QObject *parent = 0);
    QGraphicsItem * addJsonShapeItem(const QJsonObject &shape);

    const int nbDim() const { return _nbDim; }
    const int dimX() const { return _dimX; }
    const int dimY() const { return _dimY; }

    bool setDims(int dimX, int dimY);
signals:
    void changedDimX(int);
    void changedDimY(int);

public slots:
    bool setDimX(int dimX);
    bool setDimY(int dimY);

private:
    void updateDims();
};

#endif // VIBESSCENE2D_H
