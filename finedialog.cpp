#include "finedialog.h"
#include "ui_finedialog.h"

FineDialog::FineDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FineDialog)
{
    ui->setupUi(this);
}

FineDialog::~FineDialog()
{
    delete ui;
}
