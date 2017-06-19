#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QIdentityProxyModel>

class MessageModel : public QIdentityProxyModel
{
    Q_OBJECT

public:
    explicit MessageModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // MESSAGEMODEL_H
