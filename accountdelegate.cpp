#include "accountdelegate.h"
#include <QLabel>
#include <QByteArray>
#include <QPixmap>
#include <QBuffer>

AccountDelegate::AccountDelegate(QWidget* parent): QSqlRelationalDelegate(parent)
{

}
void AccountDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    if (index.column() != 10) {
        QSqlRelationalDelegate::setEditorData(editor, index);
        return;
    }
    QLabel *label = qobject_cast<QLabel*>(editor);
    if (label) {
        QByteArray imageByteArray = index.data(Qt::EditRole).toByteArray();
        QPixmap pixmap;
        if (pixmap.loadFromData(imageByteArray)) {
            label->setPixmap(pixmap);
        }
    }
}
void AccountDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const {
    if (index.column() != 10) {
        QSqlRelationalDelegate::setModelData(editor, model, index);
        return;
    }
    QLabel *label = qobject_cast<QLabel*>(editor);
    if (label) {
        QBuffer inBuffer;
        inBuffer.open(QIODevice::WriteOnly);
        if (label->pixmap()->save(&inBuffer, "PNG")) {
            model->setData(index, inBuffer.data(), Qt::EditRole);
        }
    }
}
