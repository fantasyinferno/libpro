#include "introform.h"
#include "ui_introform.h"
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QBuffer>
#include "readergui.h"

IntroForm::IntroForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IntroForm)
{
    ui->setupUi(this);
    if (parent) {
        db = dynamic_cast<ReaderGUI*>(parent)->getDatabase();
    }
}

IntroForm::~IntroForm()
{
    delete ui;
}
void IntroForm::on_pushButton_clicked()
{
    QString tdn=ui->dn_tdn->text();
    QString mk=ui->dn_mk->text();

    QString st;
    QSqlQuery query(0, db);
    query.prepare("SELECT * FROM account WHERE account = :tdn");
    query.bindValue(":tdn",tdn);
    query.exec();

    if (!query.next())
        QMessageBox::critical(this,"Account Name Error!","No Account Name found!");
    else if (mk!=query.value("password").toString())
        QMessageBox::critical(this,"Password Error!","Wrong Password!");
    else
    {
        int id = query.value("account_id").toInt();
        emit dangNhapThanhCong(id, tdn);
        ui->dn_mk->setText("");
        ui->dn_tdn->setText("");
        this->close();
    }
}


void IntroForm::setTab(int i)
{
    ui->tabWidget->setCurrentIndex(i);
    if (i == 0)
        ui->dn_tdn->setFocus();
    else ui->dk_tdn->setFocus();
}

void IntroForm::on_dangKyButton_clicked()
{




//    query("select max(user_id) as user_id from user;");

    if (ui->dk_tdn->text()=="")
    {
        QMessageBox::critical(this,"Tên đăng nhập","Tên đăng nhập trống");
        return;
    }

    QSqlQuery query2;
    query2.prepare("select password from account where account = :tdn");
    query2.bindValue(":tdn",ui->dk_tdn->text());
    query2.exec();

    if (query2.next())
    {
        QMessageBox::critical(this,"Tên đăng nhập!","Tên đăng nhập đã tồn tại");
        return;
    }

    if (ui->dk_mk->text()=="")
    {
        QMessageBox::critical(this,"Mật khẩu","Mật khẩu trống");
        return;
    }

    if (ui->dk_mk->text()!=ui->dk_nlmk->text())
    {
        QMessageBox::critical(this,"Mật khẩu","Nhập lại mật khẩu không đúng");
        return;
    }
    QSqlQuery query(0,db);
    QByteArray imageByteArray;
    QBuffer inBuffer(&imageByteArray);
    inBuffer.open(QIODevice::WriteOnly);
    ui->avatar->pixmap()->save(&inBuffer, "PNG");
    query.prepare("insert into account(account, password, status_id, fullname, identity_number, gender_id, birthdate, email, job, avatar) values(:tdn, :mk, :tt, :hvt, :cmnd, :gt, :ns, :em, :cv, :av);");
    query.bindValue(":tdn",ui->dk_tdn->text());
    query.bindValue(":mk",ui->dk_mk->text());
    query.bindValue(":tt",1);
    query.bindValue(":hvt",(ui->dk_hvt->text()!=""? ui->dk_hvt->text() : NULL));
    query.bindValue(":cmnd",(ui->dk_cmnd->text()!=""? ui->dk_cmnd->text() : NULL));
    query.bindValue(":gt",(ui->dk_gt_nam->isChecked()? "1": "2"));
    query.bindValue(":ns", ui->dk_ns->date());
    query.bindValue(":cv",(ui->dk_cv->text()!=""? ui->dk_cv->text() : NULL));
    query.bindValue(":em",(ui->dk_em->text()!=""? ui->dk_em->text() : NULL));
    query.bindValue(":av", imageByteArray);
    bool ok = query.exec();
    int id = query.lastInsertId().toInt();
    query.prepare("INSERT INTO account_role VALUES (?, ?)");
    if (ui->dk_vt_reader->isChecked()) {
        query.addBindValue(id);
        query.addBindValue(1);
        ok &= query.exec();
    }
    if (ui->dk_vt_librarian->isChecked()) {
        query.addBindValue(id);
        query.addBindValue(2);
        ok &= query.exec();
    }
    if (ui->dk_vt_manager->isChecked()) {
        query.addBindValue(id);
        query.addBindValue(3);
        ok &= query.exec();
    }
    if(ok){
        QMessageBox::about(this,"Đăng ký thành công","Đăng ký tài khoản thành công");
        this->close();
        emit dangNhapThanhCong(id, ui->dk_tdn->text());
    }
    else{
        QMessageBox::about(this,"Lỗi","Không tạo được tài khoản");
    }
}

void IntroForm::on_avatarButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), QString(), tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty()) {
        QPixmap pixmap(fileName);
        pixmap = pixmap.scaled(180, 180);
        ui->avatar->setPixmap(pixmap);
    }
}
