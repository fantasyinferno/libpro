#include <QApplication>
#include <QSqlDatabase>
#include <QDir>
#include "information.h"
#include "introform.h"
#include "mainwindow.h"
#include "about.h"
#include "inbox.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("F:/SoftwareDevelopment/Qt/libpro/libpro.db");
    if (!db.open()) {
        return -1;
    }
    MainWindow mainWindow(0, db);
    QIcon icon(":/media/images/logo.png");
    a.setWindowIcon(icon);
    mainWindow.setWindowTitle("LIBPRO");
    mainWindow.show();
    return a.exec();
}

