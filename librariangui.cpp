#include "librariangui.h"
#include "ui_librariangui.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QDebug>
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

void LibrarianGUI::on_themSachButton_clicked()
{
    // Lấy thông tin sách từ giao diện
    QString ISBN = ui->ISBN->text();
    QString title = ui->title->text();
    QString author = ui->author->text();
    int pages = ui->pages->text().toInt();
    QString year = ui->year->text();
    QString description = ui->noiDungSach->toPlainText();
    bool status = ui->khaDung->isChecked();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("F:/SoftwareDevelopment/Qt/libpro/libpro.db");
    if (!db.open()) {
        return;
    }
    QSqlQuery query(db);
    query.prepare("INSERT INTO book(book_id, title, author, pages, year, description, status) VALUES(?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(ISBN);
    query.addBindValue(title);
    query.addBindValue(author);
    query.addBindValue(pages);
    query.addBindValue(year);
    query.addBindValue(description);
    query.addBindValue(status);
//    QString queryString = QString("INSERT INTO book(book_id, title, author, pages, year, description, status) VALUES('%1', '%2', '%3', '%4', '%5', '%6', '%7')").arg(ISBN, title, author, pages, year, description, status);
    if (query.exec()) {
        ui->result->setText("Thành công!");
    } else {
        ui->result->setText("Thất bại!");
        qDebug() << query.lastError();
    }
    db.close();
}
