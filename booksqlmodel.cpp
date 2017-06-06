#include "booksqlmodel.h"
#include <QPixmap>

BookSqlModel::BookSqlModel(QObject *parent, QSqlDatabase &db): QSqlTableModel(parent, db) {

}

QVariant BookSqlModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    if (index.column() == this->fieldIndex("cover")) {
        QString imagePath = QSqlTableModel::data(index, Qt::DisplayRole).toString();
        if (role == Qt::DisplayRole)
            return QString();
        QPixmap pixmap(":" + imagePath);
        pixmap = pixmap.scaled(40, 40);

        if (role == Qt::SizeHintRole) {
            return pixmap.size();
        }
        if (role == Qt::DecorationRole) {
            return pixmap;
        }
    }
    return QSqlTableModel::data(index, role);
}
