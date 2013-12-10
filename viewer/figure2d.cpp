#include "figure2d.h"
#include <QWheelEvent>
#include <QtCore>
#include <QFileDialog>
#include <QSvgGenerator>


Figure2D::Figure2D(QWidget *parent) :
    QGraphicsView(parent)
{
    // Create a new scene
    setScene(new QGraphicsScene(this));
    this->scale(1.0, -1.0);
    this->show();
    setDragMode(ScrollHandDrag);
    // Force full viewport update (avoid problems with axes)
    setViewportUpdateMode(FullViewportUpdate);
    // Never show the scrollbars
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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
    // Gets mouse cursor in scene coordinates
    QPointF posInScene = mapToScene(event->pos());
    // Scales the view to zoom according to mouse wheel
    double s = qPow(2.0, 0.04*event->delta()/8.0);
    this->scale(s,s);
    // Centers the view where the mouse cursor was
    this->centerOn(posInScene);
}

void Figure2D::exportGraphics(QString fileName)
{
    // Open file save dialog if no filename given
    if (fileName.isEmpty())
        fileName = QFileDialog::getSaveFileName(this, tr("Export VIBes graphics"),
                QString(), tr("Portable Network Graphics (*.png);;"
                              "Joint Photographic Experts Group (*.jpg *.jpeg);;"
                              "Windows Bitmap (*.bmp);;"
                              "Scalable Vector Graphics (*.svg)"));

    // Abort if no file selected
    if (fileName.isEmpty())
        return;

    // Save as raster
    if (fileName.endsWith(".jpg", Qt::CaseInsensitive)
            || fileName.endsWith(".jpg", Qt::CaseInsensitive)
            || fileName.endsWith(".png", Qt::CaseInsensitive)
            || fileName.endsWith(".bmp", Qt::CaseInsensitive))
    {
        QImage image(this->size(), QImage::Format_ARGB32);
        image.fill(QColor(255,255,255,0));
        QPainter painter;
        painter.begin(&image);
        this->render(&painter);
        painter.end();
        image.save(fileName);
    }
    // Save as vector (SVG)
    else if (fileName.endsWith(".svg", Qt::CaseInsensitive))
    {
        QSvgGenerator generator;
        generator.setFileName(fileName);
        generator.setSize(this->size());
        generator.setViewBox(QRect(QPoint(0,0),this->size()));
        generator.setTitle(tr("VIBes figure"));
        generator.setDescription(tr("Graphics generated with VIBes on %1.").arg(QDateTime::currentDateTime().toString()));
        QPainter painter;
        painter.begin(&generator);
        this->render(&painter);
        painter.end();
    }
}
