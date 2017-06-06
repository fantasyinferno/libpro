#ifndef BOOKSQLMODEL_H
#define BOOKSQLMODEL_H

#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QTableView>
class BookSqlModel : public QSqlTableModel
{
    Q_OBJECT

public:
    BookSqlModel(QObject *parent);
    BookSqlModel(QObject* parent, QSqlDatabase &db);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};

#endif // BOOKSQLMODEL_H
