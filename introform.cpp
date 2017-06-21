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
#include <QSqlError>
#include <QBuffer>
#include <QCryptographicHash>
#include <QIntValidator>
#include <QRegExpValidator>

IntroForm::IntroForm(QWidget *parent, QSqlDatabase database) :
    QDialog(parent),
    ui(new Ui::IntroForm)
{
    ui->setupUi(this);
    this->setModal(true);
    db = database;
    QRegExp tdnReg("^[a-zA-Z-_\.0-9]{5,25}$");
    QRegExp mkReg(".{10,}");
    QRegExp emReg("\\b[a-z0-9._%+-]+@[a-z0-9.-]+\\.[a-z]{2,4}\\b");
    QRegExp hvtReg("^([^0-9]+)$");
    QRegExp cvReg(".+");
    QRegExp cmndReg("^([0-9]{9}|[0-9]{12})$");
    ui->dk_tdn->setValidator(new QRegExpValidator(tdnReg, this));
    ui->dk_mk->setValidator(new QRegExpValidator(mkReg));
    ui->dk_em->setValidator(new QRegExpValidator(emReg, this));
    ui->dk_hvt->setValidator(new QRegExpValidator(hvtReg, this));
    ui->dk_cv->setValidator(new QRegExpValidator(cvReg));
    ui->dk_cmnd->setValidator(new QRegExpValidator(cmndReg, this));

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
    query.prepare("SELECT account_id, password, status_id FROM account WHERE account = :tdn");
    query.bindValue(":tdn",tdn);
    query.exec();
    if (!query.next() || QCryptographicHash::hash(mk.toUtf8(), QCryptographicHash::Sha3_512) != query.value("password").toByteArray()) {
        QMessageBox::warning(this,"Không đúng!","Tên đăng nhập hoặc mật khẩu không đúng.");
    } else if (query.value("status_id") == 2) {
        QMessageBox::warning(this, "Bị khóa!", "Tài khoản này đã bị khóa");
    }
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
    QSqlQuery query2;
    query2.prepare("select password from account where account = :tdn");
    query2.bindValue(":tdn",ui->dk_tdn->text());
    query2.exec();
    QString errorMessage;
    if (query2.next())
    {
        errorMessage += "Tên đăng nhập đã tồn tại\n";
    }
//    if (ui->dk_tdn->text().isEmpty() || ui->dk_cv->text().isEmpty() || ui->dk_mk->text().isEmpty() ||
//            ui->dk_cmnd->text().isEmpty() || ui->dk_em->text().isEmpty() ||
//            !(ui->dk_gt_nam->isChecked() || ui->dk_gt_nu->isChecked()))
//    {
//        QMessageBox::warning(this,"Nhập lại thông tin","Vui lòng điền đẩy đủ các trường thông tin!");
//        return;
//    }
    if (!ui->dk_tdn->hasAcceptableInput()) {
        errorMessage += "Tên đăng nhập không hợp lệ (tên đăng nhập chỉ gồm các ký tự bảng chữ cái, số và các ký tự ., _, - và có độ dài từ 5 đến 25 ký tự)\n";
    }
    if (!ui->dk_mk->hasAcceptableInput()) {
        errorMessage += "Mật khẩu phải có ít nhất 10 ký tự";
    }
    if (!ui->dk_hvt->hasAcceptableInput()) {
        errorMessage += "Họ và tên không hợp lệ!\n";
    }
    if (!ui->dk_cv->hasAcceptableInput()) {
        errorMessage += "Công việc không hợp lệ!\n";
    }
    if (!ui->dk_em->hasAcceptableInput()) {
        errorMessage += "Email không hợp lệ!\n";
    }
    if (!ui->dk_cmnd->hasAcceptableInput()) {
        errorMessage += "CMND không hợp lệ";
    }
    if (ui->dk_mk->text()!=ui->dk_nlmk->text())
    {
        errorMessage += "Mật khẩu nhập lại không đúng!\n";
    }
    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, "Không hợp lệ!", errorMessage);
        return;
    }
    QSqlQuery query(0,db);
    QByteArray imageByteArray;
    QBuffer inBuffer(&imageByteArray);
    inBuffer.open(QIODevice::WriteOnly);
    ui->avatar->pixmap()->save(&inBuffer, "PNG");
    query.prepare("insert into account(account, password, status_id, fullname, identity_number, gender_id, birthdate, email, job, avatar) values(:tdn, :mk, :tt, :hvt, :cmnd, :gt, :ns, :em, :cv, :av);");
    query.bindValue(":tdn",ui->dk_tdn->text());
    QByteArray passwordByteArray = ui->dk_mk->text().toUtf8();
    passwordByteArray = QCryptographicHash::hash(passwordByteArray, QCryptographicHash::Sha3_512);
    query.bindValue(":mk",passwordByteArray);
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
//        ok &= query.exec();
    }
    if (ui->dk_vt_manager->isChecked()) {
        query.addBindValue(id);
        query.addBindValue(3);
//        ok &= query.exec();
    }
    if(ok){
        QMessageBox::about(this,"Đăng ký thành công","Đăng ký tài khoản thành công");
        // Reset form đăng ký
        ui->dk_cmnd->clear();
        ui->dk_cv->clear();
        ui->dk_em->clear();
        ui->dk_gt_nam->setChecked(false);
        ui->dk_gt_nu->setChecked(false);
        ui->dk_hvt->clear();
        ui->dk_mk->clear();
        ui->dk_nlmk->clear();
        ui->dk_ns->clear();
        ui->dk_tdn->clear();
        ui->dk_vt_librarian->setChecked(false);
        ui->dk_vt_manager->setChecked(false);
        ui->dk_vt_reader->setChecked(false);
        ui->avatar->setPixmap(QPixmap(":media/images/default.png"));
        this->close();
        emit dangNhapThanhCong(id, ui->dk_tdn->text());
    }
    else{
        QMessageBox::about(this,"Lỗi","Không tạo được tài khoản");
        qDebug() << query.lastError();
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


void IntroForm::on_formRequest(int tab) {
    this->setTab(tab);
    this->show();
}
