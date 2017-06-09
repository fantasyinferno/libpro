#include "introform.h"
#include "readergui.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QDir>
#include "librariangui.h"
#include "managergui.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ReaderGUI readerGUI;
    LibrarianGUI librarianGUI;
    ManagerGUI managerGUI;
    //managerGUI.show();
    readerGUI.show();
    QIcon icon(":/media/images/logo.png");
    a.setWindowIcon(icon);
    readerGUI.setWindowTitle("LIBPRO");
//  QObject::connect(&introForm, SIGNAL(dangNhapClicked(const QString)), &readerGUI, SLOT(on_display(const QString)));
    return a.exec();
}

