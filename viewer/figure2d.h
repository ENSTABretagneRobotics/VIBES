#ifndef FIGURE2D_H
#define FIGURE2D_H

#include <QWidget>
#include <QGraphicsView>

#include <QHash>

#include <vibesscene2d.h>
class QComboBox;
class QLabel;

class Figure2D : public QGraphicsView
{
    Q_OBJECT

    QHash<QString, QGraphicsItemGroup*> named_groups;
    QHash<QString, QGraphicsItem*> named_items;
    QComboBox *cbProjX, *cbProjY;
    QLabel *lbProjX, *lbProjY;
public:
    explicit Figure2D(QWidget *parent = 0);
    VibesScene2D* scene() const {return static_cast<VibesScene2D*>( QGraphicsView::scene() );}

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void drawForeground(QPainter *painter, const QRectF &rect);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);
    
signals:
    
public slots:
    void exportGraphics(QString fileName = QString());
    
};

#endif // FIGURE2D_H
