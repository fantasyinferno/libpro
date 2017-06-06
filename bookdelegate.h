#ifndef BOOKDELEGATE_H
#define BOOKDELEGATE_H


#include <QModelIndex>
#include <QStyleOptionViewItem>
#include <QPainter>
#include <QSqlRelationalDelegate>

class BookDelegate : public QSqlRelationalDelegate
{
    Q_OBJECT
public:
    BookDelegate(QObject *parent);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index);
};

#endif // BOOKDELEGATE_H
