#ifndef VIBESMODEL_H
#define VIBESMODEL_H

#include <QAbstractItemModel>

class Figure2D;

class VibesTreeModel : public QAbstractItemModel
{
    Q_OBJECT

    const QHash<QString, Figure2D*> * figures;
public:
    explicit VibesTreeModel(const QHash<QString, Figure2D *> &figures, QObject *parent = 0);
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    
signals:
    
public slots:
    
};

#endif // VIBESMODEL_H
