#include "introform.h"
#include "ui_introform.h"
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include "readergui.h"

IntroForm::IntroForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IntroForm)
{
    ui->setupUi(this);
    if (parent)
        db = dynamic_cast<ReaderGUI*>(parent)->getDatabase();
}

IntroForm::~IntroForm()
{
    delete ui;
}

//Sửa đường dẫn
void IntroForm::on_pushButton_clicked()
{
   // this->hide();

    QString tdn=ui->dn_tdn->text();
    QString mk=ui->dn_mk->text();

    QString st;
    QSqlQuery query(0, db);
    query.prepare("SELECT * FROM account WHERE account = :tdn");
    query.bindValue(":tdn",tdn);
    query.exec();

    QSqlRecord rec=query.record();
    int nameCol=rec.indexOf("password");
    qDebug()<<nameCol;
    if (!query.next())
        QMessageBox::critical(this,"Account Name Error!","No Account Name found!");
    else if (mk!=query.value(nameCol).toString())
        QMessageBox::critical(this,"Password Error!","Wrong Password!");
    else
    {
        int nameCol_2=rec.indexOf("role_id");
        QString role=query.value(nameCol_2).toString();
        qDebug()<<role;
        emit dangNhapThanhCong(tdn);
        ui->dn_mk->setText("");
        ui->dn_tdn->setText("");
        this->close();
    }
}


void IntroForm::setTab(int i)
{
    this->ui->tabWidget->setTabEnabled(i,1);
    this->ui->tabWidget->setTabEnabled(1-i,0);
}

//Sửa đường dẫn
void IntroForm::on_pushButton_3_clicked()
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

    query2.prepare("insert into role values(:tdn, :vt);");
    query2.bindValue(":tdn",ui->dk_tdn->text());
    query2.bindValue(":vt","reader");
    query2.exec();

    QSqlQuery query(0,db);
    query.prepare("insert into account values(null, :tdn, :mk, :tt, :hvt, :cmnd, :gt, :ns, :cv, :em);");

    query.bindValue(":tdn",ui->dk_tdn->text());
    query.bindValue(":mk",ui->dk_mk->text());
    query.bindValue(":tt",1);
    query.bindValue(":hvt",(ui->dk_hvt->text()!=""? ui->dk_hvt->text() : NULL));
    query.bindValue(":cmnd",(ui->dk_cmnd->text()!=""? ui->dk_cmnd->text() : NULL));
    query.bindValue(":gt",(ui->dk_gt_nam->isChecked()? "Nam": "Nữ"));
    query.bindValue(":ns",ui->dk_ns_ngay->text()+"/"+ui->dk_ns_thang->text()+"/"+ui->dk_ns_nam->text());
    query.bindValue(":cv",(ui->dk_cv->text()!=""? ui->dk_cv->text() : NULL));
    query.bindValue(":em",(ui->dk_em->text()!=""? ui->dk_em->text() : NULL));

    bool written = query.exec();

    if(written){
        QMessageBox::about(this,"Đăng ký thành công","Đăng ký tài khoản thành công");
        this->close();
        emit dangNhapThanhCong(ui->dk_tdn->text());
    }
    else{
        QMessageBox::about(this,"Lỗi","Không tạo được tài khoản");
    }


    db.close();
}
