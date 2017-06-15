#include <QApplication>
#include <QSqlDatabase>
#include <QDir>
#include "information.h"
#include "introform.h"
#include "accountchooser.h"
#include "readergui.h"
#include "managergui.h"
#include "librariangui.h"
#include "about.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("F:/SoftwareDevelopment/Qt/libpro/libpro.db");
    if (!db.open()) {
        return -1;
    }
    ReaderGUI readerGUI(0, db);
    LibrarianGUI librarianGUI(0, db);
    IntroForm introForm(&readerGUI, db);
    Information information(&readerGUI, db);
    AccountChooser accountChooser(&readerGUI, db);
    About about(&readerGUI);
    introForm.setWindowTitle("Đăng nhập/Đăng ký");
    information.setWindowTitle("Thông tin cá nhân");
    accountChooser.setWindowTitle("Thay đổi tài khoản");
    QObject::connect(&introForm, SIGNAL(dangNhapThanhCong(int, QString)), &readerGUI, SLOT(on_dangNhapThanhCong(int, QString)));
    QObject::connect(&introForm, SIGNAL(dangNhapThanhCong(int, QString)), &information, SLOT(on_dangNhapThanhCong(int, QString)));
    QObject::connect(&introForm, SIGNAL(dangNhapThanhCong(int, QString)), &accountChooser, SLOT(on_dangNhapThanhCong(int, QString)));
    QObject::connect(&information, SIGNAL(avatarChanged(const QPixmap*)), &readerGUI, SLOT(on_avatarChanged(const QPixmap*)));
    QObject::connect(&readerGUI, SIGNAL(updateMyBooks(const QModelIndexList&)), &information, SLOT(on_updateMyBooks(const QModelIndexList&)));
    QObject::connect(&readerGUI, SIGNAL(dangXuat()), &information, SLOT(on_dangXuat()));
    QObject::connect(&readerGUI, SIGNAL(chuyenVaiTro()), &accountChooser, SLOT(show()));
    QObject::connect(&readerGUI, SIGNAL(formRequest(int)), &introForm, SLOT(on_formRequest(int)));
    QObject::connect(&readerGUI, SIGNAL(informationRequest()), &information, SLOT(on_informationRequest()));
    QObject::connect(&readerGUI, SIGNAL(aboutTriggered()), &about, SLOT(show()));
    QObject::connect(&librarianGUI, SIGNAL(aboutTriggered()), &about, SLOT(show()));
    //managerGUI.show();
    librarianGUI.hide();
    readerGUI.show();
    QIcon icon(":/media/images/logo.png");
    a.setWindowIcon(icon);
    readerGUI.setWindowTitle("LIBPRO");
    librarianGUI.setWindowTitle("LIBPRO");
//  QObject::connect(&introForm, SIGNAL(dangNhapClicked(const QString)), &readerGUI, SLOT(on_display(const QString)));
    return a.exec();
}

