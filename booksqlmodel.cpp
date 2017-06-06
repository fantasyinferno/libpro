#include "booksqlmodel.h"

BookSqlModel::BookSqlModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

QVariant BookSqlModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

QModelIndex BookSqlModel::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex BookSqlModel::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int BookSqlModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int BookSqlModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant BookSqlModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
