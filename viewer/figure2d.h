#ifndef FIGURE2D_H
#define FIGURE2D_H

#include <QGraphicsView>
#include <QGraphicsScene>

#include <QHash>

class Figure2D : public QGraphicsView
{
    Q_OBJECT

    QHash<QString, QGraphicsItemGroup*> named_groups;
    QHash<QString, QGraphicsItem*> named_items;

public:
    explicit Figure2D(QWidget *parent = 0);

signals:

public slots:

};

#endif // FIGURE2D_H
