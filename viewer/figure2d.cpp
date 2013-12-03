#include "figure2d.h"
#include <QWheelEvent>
#include <QtCore>


Figure2D::Figure2D(QWidget *parent) :
    QGraphicsView(parent)
{
    // Create a new scene
    setScene(new QGraphicsScene(this));
    this->scale(1.0, -1.0);
    this->show();
    setDragMode(ScrollHandDrag);
}

void Figure2D::drawForeground(QPainter *painter, const QRectF &rect)
{

    int log_scale_x = floor(log10(rect.height()/3.0)*3.0);
    double scale_x = pow(10.0, floor((double)log_scale_x/3));
    switch (log_scale_x%3) {
    case 0: break;
    case 1: case-2: scale_x *= 2.0; break;
    case 2: case-1: scale_x *= 5.0; break;
    }

    int log_scale_y = floor(log10(rect.height()/3.0)*3.0);
    double scale_y = pow(10.0, floor((double)log_scale_y/3));
    switch (log_scale_y%3) {
    case 0: break;
    case 1: case-2: scale_y *= 2.0; break;
    case 2: case-1: scale_y *= 5.0; break;
    }

    double x0 = ceil(rect.left() / scale_x) * scale_x;
    double y0 = ceil(qMin(rect.bottom(),rect.top()) / scale_y) * scale_y;

    painter->setTransform(QTransform());

    for (double xtick=x0; xtick<rect.right(); xtick+=scale_x)
    {
        double x_wnd = mapFromScene(xtick,0).x();

        painter->setPen(QColor(0,0,0));
        painter->drawLine(x_wnd,0,x_wnd,5);
        painter->drawText(x_wnd+4,10, QString("%1").arg(xtick));
    }

    for (double ytick=y0; ytick<qMax(rect.top(),rect.bottom()); ytick+=scale_y)
    {
        double y_wnd = mapFromScene(0,ytick).y();

        painter->setPen(QColor(0,0,0));
        painter->drawLine(0,y_wnd,5,y_wnd);
        painter->drawText(2, y_wnd+12, QString("%1").arg(ytick));
    }
}

void Figure2D::wheelEvent(QWheelEvent *event)
{
    double s = qPow(2.0, 0.05*event->delta()/8.0);
    this->scale(s,s);
}
