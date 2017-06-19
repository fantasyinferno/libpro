#ifndef ACCOUNTDELEGATE_H
#define ACCOUNTDELEGATE_H
#include <QWidget>
#include <QSqlRelationalDelegate>
#include <QModelIndex>
#include <QAbstractItemModel>


class AccountDelegate : public QSqlRelationalDelegate
{
public:
    AccountDelegate(QWidget*);
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const;
};

#endif // ACCOUNTDELEGATE_H
