#include "accountchooser.h"
#include "ui_accountchooser.h"

AccountChooser::AccountChooser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AccountChooser)
{
    ui->setupUi(this);
}

AccountChooser::~AccountChooser()
{
    delete ui;
}
