#include <QApplication>
#include <QSqlDatabase>
#include <QDir>
#include "information.h"
#include "introform.h"
#include "accountchooser.h"
#include "mainwindow.h"
#include "about.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("F:/SoftwareDevelopment/Qt/libpro/libpro.db");
    if (!db.open()) {
        return -1;
    }
    MainWindow mainWindow(0, db);
    IntroForm introForm(&mainWindow, db);
    Information information(&mainWindow, db);
    About about(&mainWindow);
    introForm.setWindowTitle("Đăng nhập/Đăng ký");
    information.setWindowTitle("Thông tin cá nhân");
    about.setWindowTitle("Về LIBPRO");
    QObject::connect(&introForm, SIGNAL(dangNhapThanhCong(int, QString)), &mainWindow, SLOT(on_dangNhapThanhCong(int, QString)));
    QObject::connect(&introForm, SIGNAL(dangNhapThanhCong(int, QString)), &information, SLOT(on_dangNhapThanhCong(int, QString)));
    QObject::connect(&information, SIGNAL(avatarChanged(const QPixmap*)), &mainWindow, SLOT(on_avatarChanged(const QPixmap*)));
    QObject::connect(&mainWindow, SIGNAL(updateMyBooks(const QModelIndexList&)), &information, SLOT(on_updateMyBooks(const QModelIndexList&)));
    QObject::connect(&mainWindow, SIGNAL(dangXuat()), &information, SLOT(on_dangXuat()));
    QObject::connect(&mainWindow, SIGNAL(formRequest(int)), &introForm, SLOT(on_formRequest(int)));
    QObject::connect(&mainWindow, SIGNAL(informationRequest()), &information, SLOT(on_informationRequest()));
    QObject::connect(&mainWindow, SIGNAL(aboutTriggered()), &about, SLOT(show()));
    QObject::connect(&information, SIGNAL(rolesLoaded(QList<int>)), &mainWindow, SLOT(on_rolesLoaded(QList<int>)));
    QIcon icon(":/media/images/logo.png");
    a.setWindowIcon(icon);
    mainWindow.setWindowTitle("LIBPRO");
    mainWindow.show();
    return a.exec();
}

