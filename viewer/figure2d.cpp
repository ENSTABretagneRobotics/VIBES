#include "figure2d.h"

Figure2D::Figure2D(QWidget *parent) :
    QGraphicsView(parent)
{
    // Create a new scene
    setScene(new QGraphicsScene(this));
}
