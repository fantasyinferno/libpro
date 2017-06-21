#include "finedialog.h"
#include "ui_finedialog.h"
#include "inbox.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>
FineDialog::FineDialog(QWidget *parent, QSqlDatabase database, QModelIndexList list, Inbox *theInbox) :
    QDialog(parent),
    ui(new Ui::FineDialog)
{
    ui->setupUi(this);
    this->setModal(true);
    db = database;
    // Lấy thông tin các hàng được chọn
    selected = list;
    inbox = theInbox;
    // Giấu soTien
    on_hinhThuc_currentIndexChanged(ui->hinhThuc->currentIndex());
}

FineDialog::~FineDialog()
{
    delete ui;
}

void FineDialog::on_hinhThuc_currentIndexChanged(int index)
{
    if (index != 2) {
        ui->soTien->setEnabled(false);
    } else {
        ui->soTien->setEnabled(true);
    }
}

void FineDialog::on_chapNhan_clicked()
{
    QString method = ui->hinhThuc->currentText();
    bool ok = true;
    for (QModelIndex &i: selected) {
        // index của account là 1, của book là 2
        QString request_id = i.sibling(i.row(), 0).data().toString();
        QString accountName = i.sibling(i.row(), 1).data().toString();
        QString bookName = i.sibling(i.row(), 2).data().toString();
        QString title = QString("LIBPRO - %1 %2 đối với đơn hàng #%3 (%4)").arg(method, accountName, request_id, bookName);
        QString message = ui->noiDung->toPlainText();

        if (method == "Phạt hành chính") {
            message += "\nTổng số tiền phạt hành chính: " + ui->soTien->text();
        }
        ok &= inbox->sendMessage(accountName, title, message);
        if (!ok) {
            break;
        }
    }
    if (ok) {
        QMessageBox::information(this, "Phạt", "Phạt thành công!");
    } else {
        QMessageBox::warning(this, "Phạt", "Phạt thất bại");
    }
}

void FineDialog::on_huyBo_clicked()
{
    this->close();
}
