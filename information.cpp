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
#include <QSqlRelation>
#include "readergui.h"

Information::Information(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Information)
{
    ui->setupUi(this);
    db = dynamic_cast<ReaderGUI*>(parent)->getDatabase();
    enableEdit(false);
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

    query.prepare("select account_id from account where account = :tdn;");
    query.bindValue(":tdn", tdn);
    query.exec();
    query.next();
    int id = query.value(0).toInt();

    query.prepare("SELECT * FROM account_role WHERE account_id = :id");
    query.bindValue(":id", id);
    query.exec();

    while (query.next()) {
        int role = query.value(1).toInt();
        if (role == 1) {
            ui->ip_vt_reader->setChecked(true);
        } else if (role == 2) {
            ui->ip_vt_librarian->setChecked(true);
        } else {
            ui->ip_vt_manager->setChecked(true);
        }
    }
}

//*************************************************************
//Thay đổi vai trò

//*********************************
void Information::submitVt()
{
    QSqlQuery query(0,db);



    query.prepare("select account_id from account where account = :tdn;");
    query.bindValue(":tdn", tdn);
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
}

void Information::on_thayDoiButton_clicked()
{
    enableEdit();
}
void Information::on_dangNhapThanhCong(QString username) {
    tdn = username;
    ui->username->setText(tdn);
    model = new QSqlRelationalTableModel(0, db);
    model->setTable("account");
    int genderIdx = model->fieldIndex("gender_id");
    int statusIdx = model->fieldIndex("status_id");
    model->setRelation(genderIdx, QSqlRelation("gender", "gender_id", "gender"));
    model->setRelation(statusIdx, QSqlRelation("status", "status_id", "status"));
    model->setFilter("account = '" + tdn + "'");
    model->select();

    QSqlTableModel *relModelGender = model->relationModel(genderIdx);
    QSqlTableModel *relModelStatus = model->relationModel(statusIdx);
    ui->ip_gt->setModel(relModelGender);
    ui->ip_gt->setModelColumn(relModelGender->fieldIndex("gender"));
    ui->ip_tt->setModel(relModelStatus);
    ui->ip_tt->setModelColumn(relModelStatus->fieldIndex("status"));
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(ui->ip_mk, model->fieldIndex("password"));
    mapper->addMapping(ui->ip_tt, statusIdx);
    mapper->addMapping(ui->ip_hvt, model->fieldIndex("fullname"));
    mapper->addMapping(ui->ip_cmnd, model->fieldIndex("identity_number"));
    mapper->addMapping(ui->ip_gt, genderIdx);
    mapper->addMapping(ui->ip_nn, model->fieldIndex("job"));
    mapper->addMapping(ui->ip_em, model->fieldIndex("email"));
    mapper->addMapping(ui->ip_ns, model->fieldIndex("birthdate"));
    mapper->toFirst();
    checkVt();
}

void Information::on_huyButton_clicked()
{
    enableEdit(false);
}
