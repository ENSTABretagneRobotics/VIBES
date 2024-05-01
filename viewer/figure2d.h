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

    // bool to indicate if axis needs to be drawn or not
    bool showAxis;

    int fontSize;
    int xTicksSpacing;
    int yTicksSpacing;
signals:

public slots:
    void exportGraphics(QString fileName = QString());
    void setShowAxis(bool value) { showAxis = value;}

protected slots:
    void refreshProjectionSelectors();

};

#endif // FIGURE2D_H
