#include "bookdelegate.h"

#include <QPixmap>
#include <QString>
#include <QStyle>
#include <QColor>
#include <QLabel>
BookDelegate::BookDelegate(QObject *parent): QSqlRelationalDelegate(parent)
{

}

void BookDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (index.column() == 1) {
        QString imagePath = index.data().toString();
        QPixmap image(":" + imagePath);
        int x = option.rect.x();
        int y = option.rect.y();
        int w = 30;
        int h = option.rect.height();
        drawBackground(painter, option, index);
        drawFocus(painter, option, option.rect);
        painter->drawPixmap(x + option.rect.width() / 2 - w / 2, y, w, h, image);
    } else if (index.column() == 7) {
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
void BookDelegate::setEditorData(QWidget *editor, const QModelIndex &index) {
    if (index.column() != 1) {
         QSqlRelationalDelegate::setEditorData(editor, index);
         return;
    }
    QLabel *lab = qobject_cast<QLabel *>(editor);
    QString imagePath = index.data().toString();
    QPixmap pixmap(":" + imagePath);
    lab->setPixmap(pixmap);
}
