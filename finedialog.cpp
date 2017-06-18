#include "finedialog.h"
#include "ui_finedialog.h"

FineDialog::FineDialog(QWidget *parent, QSqlDatabase database, QModelIndexList list) :
    QDialog(parent),
    ui(new Ui::FineDialog)
{
    ui->setupUi(this);
    db = database;
    // Lấy thông tin các hàng được chọn
    selected = list;
}

FineDialog::~FineDialog()
{
    delete ui;
}

void FineDialog::on_hinhThuc_currentIndexChanged(int index)
{
    if (index != 2 && !ui->soTien->isHidden()) {
        ui->soTien->hide();
    } else {
        ui->soTien->show();
    }
}
