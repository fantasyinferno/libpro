#include "information.h"
#include "ui_information.h"
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QSqlRelationalDelegate>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include <QAction>
#include <QFileDialog>
#include <QBuffer>
#include <QSqlRelation>
#include <QList>
#include "accountdelegate.h"
#include "readergui.h"

Information::Information(QWidget *parent, QSqlDatabase database) :
    QDialog(parent),
    ui(new Ui::Information)
{
    ui->setupUi(this);
    this->setModal(true);
    db = database;
    enableEdit(false);
    // Model cho thông tin cá nhân
    model = new QSqlRelationalTableModel(0, db);
    model->setTable("account");
    int genderIdx = model->fieldIndex("gender_id");
    int statusIdx = model->fieldIndex("status_id");
    model->setRelation(genderIdx, QSqlRelation("gender", "gender_id", "gender"));
    model->setRelation(statusIdx, QSqlRelation("status", "status_id", "status"));

    // Tên của cột gender_id và status_id đã thay đổi thành gender và status
    QSqlTableModel *relModelGender = model->relationModel(genderIdx);
    QSqlTableModel *relModelStatus = model->relationModel(statusIdx);
    ui->ip_gt->setModel(relModelGender);
    ui->ip_gt->setModelColumn(relModelGender->fieldIndex("gender"));
    ui->ip_tt->setModel(relModelStatus);
    ui->ip_tt->setModelColumn(relModelStatus->fieldIndex("status"));

    // Mapper để liên hệ các trường vào cơ sở dữ liệu
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setItemDelegate(new AccountDelegate(this));
    mapper->addMapping(ui->avatar, model->fieldIndex("avatar"));
    mapper->addMapping(ui->ip_mk, model->fieldIndex("password"));
    mapper->addMapping(ui->ip_tt, statusIdx);
    mapper->addMapping(ui->ip_hvt, model->fieldIndex("fullname"));
    mapper->addMapping(ui->ip_cmnd, model->fieldIndex("identity_number"));
    mapper->addMapping(ui->ip_gt, genderIdx);
    mapper->addMapping(ui->ip_nn, model->fieldIndex("job"));
    mapper->addMapping(ui->ip_em, model->fieldIndex("email"));
    mapper->addMapping(ui->ip_ns, model->fieldIndex("birthdate"));
    connect(mapper, SIGNAL(currentIndexChanged(int)), this, SLOT(on_currentIndexChanged(int)));

    // Model cho sách đã mượn
    bookModel = new QSqlRelationalTableModel(this);
    bookModel->setTable("account_book");
}
void Information::on_informationRequest() {
    this->show();
}

void Information::load(QString tendangnhap, QString vaitro)
{

    QSqlQuery query(0, db);

    query.prepare("select * from account where account=:tdn");
    query.bindValue(":tdn",tendangnhap);
    query.exec();

    QSqlRecord rec=query.record();
    query.next();

    query.prepare("select role from role where account =:tdn;");
    query.bindValue(":tdn",tendangnhap);
    query.exec();
    query.next();
    if (query.next())
    {
    }
    if (query.next())
    {
    }

    if (!(vaitro=="Manager"))
    {
    }

}

Information::~Information()
{
    delete ui;
}

void Information::checkVt() {
    QSqlQuery query(0, db);


    query.prepare("SELECT role_id FROM account_role WHERE account_id = :id");
    query.bindValue(":id", user_id);
    query.exec();

    while (query.next()) {
        int role = query.value(0).toInt();
        if (role == 1) {
            ui->ip_vt_reader->setChecked(true);
        } else if (role == 2) {
            ui->ip_vt_librarian->setChecked(true);
        } else {
            ui->ip_vt_manager->setChecked(true);
        }
        rolesList.append(role);
    }
    emit rolesLoaded(rolesList);
}

//*************************************************************
//Thay đổi vai trò

//*********************************
void Information::submitVt()
{
    QSqlQuery query(0,db);



    query.prepare("select account_id from account where account = :tdn;");
    query.bindValue(":tdn", user);
    query.exec();
    query.next();
    int id = query.value(0).toInt();

    query.prepare("delete from account_role where account_id = :id;");
    query.bindValue(":id", id);
    query.exec();
    bool ms;
    // TODO: replace account's name with it's id. Connect checkboxes to the model.
    if (ui->ip_vt_librarian->isChecked())
    {
        query.prepare("insert into account_role values(:id, :role);");
        query.bindValue(":id",id);
        query.bindValue(":role", 2);
        ms=query.exec();
    }

    if (ui->ip_vt_manager->isChecked())
    {
        query.prepare("insert into account_role values(:id, :role);");
        query.bindValue(":id",id);
        query.bindValue(":role", 3);
        ms=query.exec();
    }

    if (ui->ip_vt_reader->isChecked())
    {
        query.prepare("insert into account_role values(:id, :role);");
        query.bindValue(":id",id);
        query.bindValue(":role", 1);
        ms=query.exec();
    }

    if (ms)
    {
        QMessageBox::about(this,"Cập nhật","Cập nhật thành công");
    }
    else
    {
        QMessageBox::about(this,"Cập nhật","Cập nhật thất bại");
        qDebug() << query.lastError();
    }
}
void Information::submitAv() {
    QByteArray imageByteArray;
    QBuffer inBuffer(&imageByteArray);
    inBuffer.open(QIODevice::WriteOnly);
    ui->avatar->pixmap()->save(&inBuffer, "PNG");
    QSqlQuery query(0, db);
    query.prepare("UPDATE account SET avatar = ? WHERE account_id = ?");
    query.addBindValue(imageByteArray);
    query.addBindValue(user_id);
    if (!query.exec()) {
        qDebug() << query.lastError();
    }
}

//*********************************


void Information::on_hoanTatButton_clicked()
{
    enableEdit(false);
    mapper->submit();
    submitVt();
}

void Information::enableEdit(bool enabled = true) {
    ui->ip_cmnd->setEnabled(enabled);
    ui->ip_em->setEnabled(enabled);
    ui->ip_gt->setEnabled(enabled);
    ui->ip_hvt->setEnabled(enabled);
    ui->ip_mk->setEnabled(enabled);
    ui->ip_nn->setEnabled(enabled);
    ui->ip_ns->setEnabled(enabled);
    ui->ip_tt->setEnabled(enabled);
    ui->ip_vt_librarian->setEnabled(enabled);
    ui->ip_vt_manager->setEnabled(enabled);
    ui->ip_vt_reader->setEnabled(enabled);
    ui->hoanTatButton->setEnabled(enabled);
    ui->huyButton->setEnabled(enabled);
    ui->thayDoiButton->setEnabled(!enabled);
    ui->avatarButton->setEnabled(enabled);
}

void Information::on_thayDoiButton_clicked()
{
    enableEdit();
}
void Information::on_dangNhapThanhCong(int id, QString username) {
    user = username;
    ui->username->setText(user);
    user_id = id;
    model->setFilter(QString("account_id = '%1'").arg(user_id)); // SQL Injection Alert!
    model->select();

    mapper->toFirst();
    // Gọi hàm kiểm tra vai trò
    checkVt();
    bookModel->setFilter(QString("account_id = %1").arg(user_id)); // SQL Injection Alert!
    bookModel->setRelation(bookModel->fieldIndex("book_id"), QSqlRelation("book", "book_id", "title"));
    bookModel->setHeaderData(1, Qt::Horizontal, "Tựa đề");
    bookModel->setHeaderData(2, Qt::Horizontal,  "Ngày mượn");
    bookModel->setHeaderData(3, Qt::Horizontal, "Ngày hết hạn");
    bookModel->select();
    ui->sachDaMuon->setModel(bookModel);
    ui->sachDaMuon->setItemDelegate(new QSqlRelationalDelegate(this));
    ui->sachDaMuon->setColumnHidden(0, true);
}
void Information::on_huyButton_clicked()
{
    enableEdit(false);
    mapper->revert();
//    QByteArray imageByteArray = model->data(model->index(0, model->fieldIndex("avatar"))).toByteArray();
//    QPixmap pixmap;
//    pixmap.loadFromData(imageByteArray);
//    ui->avatar->setPixmap(pixmap);
}
void Information::on_updateMyBooks(const QModelIndexList& selectedList) {
    QSqlQuery query(0, db);
    query.prepare("INSERT INTO account_book(account_id, book_id, start_date, due_date) VALUES(:account_id, :book_id, :start_date, :due_date)");
    query.bindValue(":account_id", user_id);
    QDate today = QDate::currentDate();
    for (int i = 0; i != selectedList.size(); ++i) {
        QString book_id = selectedList[i].data().toString();
        query.bindValue(":book_id", book_id);
        query.bindValue(":start_date", today);
        query.bindValue(":due_date", today.addDays(15));
        if (!query.exec())
            qDebug() << query.lastError();
    }
    bookModel->select();
}

void Information::on_avatarButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), QString(), tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty()) {
        QPixmap pixmap(fileName);
        pixmap = pixmap.scaled(180, 180);
        ui->avatar->setPixmap(pixmap);
    }
}
void Information::on_currentIndexChanged(int i) {
    emit avatarChanged(ui->avatar->pixmap());
}
void Information::on_dangXuat() {
    model->setFilter("0");
    bookModel->setFilter("0");
    model->select();
    bookModel->select();
    mapper->toFirst();
}

void Information::on_iAmYourParent(QWidget *widget)
{
    this->setParent(widget);
    this->setWindowFlags(Qt::Dialog);
}
