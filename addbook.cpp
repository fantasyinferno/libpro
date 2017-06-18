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
AddBook::AddBook(QWidget *parent, QSqlDatabase database) :
    QDialog(parent),
    ui(new Ui::AddBook)
{
    ui->setupUi(this);
    // Thiết lập cơ sở dữ liệu
    db = database;
    // Thiết lập query
}

AddBook::~AddBook()
{
    delete ui;
}

void AddBook::on_submitButton_clicked()
{
    QSqlQuery query(0, db);
    query.prepare("INSERT INTO book(book_id, cover, title, author, pages, year, description, status) VALUES(?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(ui->ISBN->text());
    QByteArray imageByteArray;
    QBuffer inBuffer(&imageByteArray);
    inBuffer.open(QIODevice::WriteOnly);
    ui->cover->pixmap()->save(&inBuffer, "PNG");
    query.addBindValue(imageByteArray);
    qDebug() << imageByteArray;
    query.addBindValue(ui->title->text());
    qDebug() << ui->title->text();
    query.addBindValue(ui->author->text());
    qDebug() << ui->author->text();
    query.addBindValue(ui->pages->text().toInt());
    qDebug() << ui->pages->text().toInt();
    query.addBindValue(ui->year->text());
    qDebug() << ui->year->text();
    query.addBindValue(ui->description->toPlainText());
    qDebug() << ui->description->toPlainText();
    query.addBindValue(ui->status->isChecked());
    qDebug() << ui->status->isChecked();
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
        pixmap.scaled(128, 128);
        ui->cover->setPixmap(pixmap);
    }
}
