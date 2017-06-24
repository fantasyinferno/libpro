#include "addbook.h"
#include "ui_addbook.h"
#include "bookdelegate.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QByteArray>
#include <QBuffer>
#include <QRegExp>
#include <QRegExpValidator>
#include <QIntValidator>
AddBook::AddBook(QWidget *parent, QSqlDatabase database) :
    QDialog(parent),
    ui(new Ui::AddBook)
{
    ui->setupUi(this);
    // Thiết lập cơ sở dữ liệu
    db = database;
    // Thiết lập query
    QRegExp isbnReg("(^[A-Z0-9]{10}$)|(^[A-Z0-9]{13}$)");
    QRegExp titleReg(".+");
    QRegExp authorReg("^([^0-9]+)$");
    QRegExp yearReg("^[1-9][0-9]{1,3}$");
    QRegExp pagesReg("^[1-9][0-9]+$");
    ui->ISBN->setValidator(new QRegExpValidator(isbnReg));
    ui->title->setValidator(new QRegExpValidator(titleReg));
    ui->author->setValidator(new QRegExpValidator(authorReg));
    ui->year->setValidator(new QRegExpValidator(yearReg));
    ui->pages->setValidator(new QRegExpValidator(pagesReg));
}

AddBook::~AddBook()
{
    delete ui;
}

void AddBook::on_submitButton_clicked()
{
    QString errorMessage = "";
    if (!ui->ISBN->hasAcceptableInput()) {
        errorMessage += "Mã ISBN không hợp lệ\n";
    }
    if (!ui->title->hasAcceptableInput()) {
        errorMessage += "Tiêu đề không hợp lệ\n";
    }
    if (!ui->author->hasAcceptableInput()) {
        errorMessage += "Tác giả không hợp lệ\n";
    }
    if (!ui->year->hasAcceptableInput()) {
        errorMessage += "Năm sảm xuất không hợp lệ\n";
    }
    if (!ui->pages->hasAcceptableInput()) {
        errorMessage += "Số trang không hợp lệ";
    }
    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, "Không hợp lệ", errorMessage);
        return;
    }
    QSqlQuery query(0, db);
    query.prepare("INSERT INTO book(book_id, cover, title, author, pages, year, description, status) VALUES(?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(ui->ISBN->text());
    QByteArray imageByteArray;
    QBuffer inBuffer(&imageByteArray);
    inBuffer.open(QIODevice::WriteOnly);
    ui->cover->pixmap()->save(&inBuffer, "PNG");
    query.addBindValue(imageByteArray);
    query.addBindValue(ui->title->text());
    query.addBindValue(ui->author->text());
    query.addBindValue(ui->pages->text().toInt());
    query.addBindValue(ui->year->text());
    query.addBindValue(ui->description->toPlainText());
    query.addBindValue(ui->status->isChecked());
    if (query.exec()) {
        QMessageBox::information(this, "Thành công", "Bạn đã thêm sách thành công!");
        emit accepted();
    } else {
        QMessageBox::information(this, "Thất bại", "Đã có lỗi xảy ra!");
        qDebug() << query.lastError();
    }
}

void AddBook::on_cancelButton_clicked()
{
    this->close();
}

void AddBook::on_changeCoverButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), QString(), tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty()) {
        QPixmap pixmap(fileName);
        pixmap = pixmap.scaled(128, 128);
        ui->cover->setPixmap(pixmap);
    }
}
