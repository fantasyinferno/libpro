#include "librariangui.h"
#include "ui_librariangui.h"

LibrarianGUI::LibrarianGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LibrarianGUI)
{
    ui->setupUi(this);
}

LibrarianGUI::~LibrarianGUI()
{
    delete ui;
}
