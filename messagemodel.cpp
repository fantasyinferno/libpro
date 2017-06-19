#include "messagemodel.h"
#include <QFont>


MessageModel::MessageModel(QObject *parent)
    : QIdentityProxyModel(parent)
{
}

QVariant MessageModel::data(const QModelIndex &index, int role) const
{
    // Nếu cột thứ 6 (is_read) của dòng là 0 thì in đậm cả dòng
    if (role == Qt::FontRole && index.sibling(index.row(), 6).data().toInt() == 0) {
            QFont bold = QFont();
            bold.setBold(true);
            return bold;
    }
    // Sử dụng QIdentityProxyModel đối với các role khác
    return QIdentityProxyModel::data(index, role);
}


