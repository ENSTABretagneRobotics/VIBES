#ifndef FIGURE2D_H
#define FIGURE2D_H

#include <QWidget>
#include <QGraphicsView>

#include <QHash>

class Figure2D : public QGraphicsView
{
    Q_OBJECT

    QGraphicsScene _scene;
    QHash<QString, QGraphicsItemGroup*> named_groups;
    QHash<QString, QGraphicsItem*> named_items;
public:
    explicit Figure2D(QWidget *parent = 0);

protected:
    void drawForeground(QPainter *painter, const QRectF &rect);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    
signals:
    
public slots:
    void exportGraphics(QString fileName = QString());
    
};

#endif // FIGURE2D_H
