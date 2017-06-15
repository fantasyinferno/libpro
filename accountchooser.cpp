#include "accountchooser.h"
#include "ui_accountchooser.h"
#include <QList>
#include <QDebug>
#include <QSqlQuery>
AccountChooser::AccountChooser(QWidget *parent, QSqlDatabase database) :
    QDialog(parent),
    ui(new Ui::AccountChooser)
{
    ui->setupUi(this);
    this->setModal(true);
    db = database;
}

AccountChooser::~AccountChooser()
{
    delete ui;
}

AccountChooser::on_dangNhapThanhCong(int id, QString username) {
    QSqlQuery query(0, db);
    query.prepare("SELECT role_id FROM account_role WHERE account_id = ?");
    query.addBindValue(id);
    if (query.exec()) {
        while (query.next()) {
            int role_id = query.value(0).toInt();
            qDebug() << role_id;
            roleList.append(role_id);
        }
        if (roleList.contains(1)) {
            ui->docGiaButton->setEnabled(true);
        }
        if (roleList.contains(2)) {
            ui->thuThuButton->setEnabled(true);
        }
        if (roleList.contains(3)) {
            ui->quanLyButton->setEnabled(true);
        }
    }
}
