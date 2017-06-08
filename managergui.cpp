#include "managergui.h"
#include "ui_managergui.h"

ManagerGUI::ManagerGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManagerGUI)
{
    ui->setupUi(this);
}

ManagerGUI::~ManagerGUI()
{
    delete ui;
}
