#include "managergui.h"
#include "ui_managergui.h"

ManagerGUI::ManagerGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManagerGUI)
{
    // Do something here
    ui->setupUi(this);
}

ManagerGUI::~ManagerGUI()
{
    delete ui;
}
