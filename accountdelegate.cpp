#include "accountdelegate.h"
#include <QLabel>
#include <QByteArray>
#include <QPixmap>
#include <QBuffer>
#include <QPainter>
AccountDelegate::AccountDelegate(QWidget* parent): QSqlRelationalDelegate(parent)
{

}

void AccountDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (index.column() == 10) {
        // 10 là cột avatar
        QByteArray imageByteArray = index.data().toByteArray();
        QPixmap image;
        image.loadFromData(imageByteArray);
        int x = option.rect.x();
        int y = option.rect.y();
        int w = 30;
        int h = option.rect.height();
        drawBackground(painter, option, index);
        drawFocus(painter, option, option.rect);
        painter->drawPixmap(x + option.rect.width() / 2 - w / 2, y, w, h, image);
    } else {
        QSqlRelationalDelegate::paint(painter, option, index);
    }
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
