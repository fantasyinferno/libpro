#include "bookdelegate.h"

#include <QPixmap>
#include <QString>
#include <QStyle>
#include <QColor>
#include <QLabel>
#include <QByteArray>
#include <QBuffer>
BookDelegate::BookDelegate(QObject *parent): QSqlRelationalDelegate(parent)
{

}

void BookDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (index.column() == 1) {
        // 1 là cột bìa sách
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
    } else if (index.column() == 7) {
        // 7 là cột trạng thái
        drawBackground(painter, option, index);
        drawFocus(painter, option, option.rect);
        if (index.data() == 1)
            drawDisplay(painter, option, option.rect, "Khả dụng");
        else drawDisplay(painter, option, option.rect, "Không khả dụng");
    }
    else {
        QSqlRelationalDelegate::paint(painter, option, index);
    }
}
void BookDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    if (index.column() != 1) {
         return QSqlRelationalDelegate::setEditorData(editor, index);
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

void BookDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const {
    if (index.column() != 1) {
        return QSqlRelationalDelegate::setModelData(editor, model, index);
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
