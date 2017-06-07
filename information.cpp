#include "information.h"
#include "ui_information.h"
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QMessageBox>
#include <QDebug>

Information::Information(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Information)
{
    ui->setupUi(this);

    ui->ip_mk->hide();
    ui->ok_mk->hide();
    ui->ca_mk->hide();

    ui->ip_tt->hide();
    ui->ok_tt->hide();
    ui->ca_tt->hide();

    ui->ip_hvt->hide();
    ui->ok_hvt->hide();
    ui->ca_hvt->hide();

    ui->ip_cmnd->hide();
    ui->ok_cmnd->hide();
    ui->ca_cmnd->hide();

    ui->ip_gt_nam->hide();
    ui->ip_gt_nu->hide();
    ui->ok_gt->hide();
    ui->ca_gt->hide();

    ui->ip_ns_ngay->hide();
    ui->ip_ns_thang->hide();
    ui->ip_ns_nam->hide();
    ui->ok_ns->hide();
    ui->ca_ns->hide();

    ui->ip_nn->hide();
    ui->ok_nn->hide();
    ui->ca_nn->hide();

    ui->ip_em->hide();
    ui->ok_em->hide();
    ui->ca_em->hide();

    ui->ip_vt_librarian->hide();
    ui->ip_vt_manager->hide();
    ui->ip_vt_reader->hide();
    ui->op_vt_2->hide();
    ui->op_vt_3->hide();
    ui->ok_vt->hide();
    ui->ca_vt->hide();
}

void Information::load(QString tendangnhap, QString vaitro)
{
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/pc/Documents/QT/Ass2/Huy/libpro/libpro.db");
    if (!db.open()) {
        return;
    }

    QSqlQuery query(0, db);

    query.prepare("select * from account where account=:tdn");
    query.bindValue(":tdn",tendangnhap);
    query.exec();

    QSqlRecord rec=query.record();
    query.next();

    ui->op_tdn->  setText(query.value(rec.indexOf("account")).toString());
    ui->op_mk->   setText(query.value(rec.indexOf("password")).toString());
    ui->op_tt->   setText(query.value(rec.indexOf("status")).toString());
    ui->op_hvt->  setText(query.value(rec.indexOf("user_name")).toString());
    ui->op_cmnd-> setText(query.value(rec.indexOf("identity_id")).toString());
    ui->op_gt->   setText(query.value(rec.indexOf("sex")).toString());
    ui->op_ns->   setText(query.value(rec.indexOf("birthdate")).toString());
    ui->op_nn->   setText(query.value(rec.indexOf("job")).toString());
    ui->op_em->   setText(query.value(rec.indexOf("email")).toString());

    query.prepare("select role from role where account =:tdn;");
    query.bindValue(":tdn",tendangnhap);
    query.exec();

    ui->op_vt_2->hide();
    ui->op_vt_3->hide();
    query.next();
    ui->op_vt_1->setText(query.value(0).toString());
    if (query.next())
    {
        ui->op_vt_2->setText(query.value(0).toString());
        ui->op_vt_2->show();
    }
    if (query.next())
    {
        ui->op_vt_3->setText(query.value(0).toString());
        ui->op_vt_3->show();
    }

    if (!(vaitro=="Manager"))
    {
        ui->td_vt->hide();
        ui->td_tt->hide();
    }

}

Information::~Information()
{
    delete ui;
}

//*************************************************************
//Thay đổi mật khẩu

void Information::on_td_mk_clicked()
{
    ui->ip_mk->show();
    ui->ok_mk->show();
    ui->ca_mk->show();
}

//*********************************
void Information::on_ok_mk_clicked()
{
    if (ui->ip_mk->text()=="")
    {
        QMessageBox::critical(this,"Mật khẩu","Mật khẩu trống");
        return;
    }

    QSqlQuery query(0,db);
    QString tdn=ui->op_tdn->text();

    query.prepare("update account set password = :newpass where account = :tdn;");
    query.bindValue(":newpass",ui->ip_mk->text());
    query.bindValue(":tdn",tdn);
    bool ms=query.exec();

    if (ms)
    {
        QMessageBox::about(this,"Cập nhật","Cập nhật thành công");
    }
    else
    {
        QMessageBox::about(this,"Cập nhật","Cập nhật thất bại");
    }

    ui->ip_mk->setText("");
    ui->ip_mk->hide();
    ui->ok_mk->hide();
    ui->ca_mk->hide();
    load(tdn,"reader");

}

//*********************************
void Information::on_ca_mk_clicked()
{
    ui->ip_mk->setText("");
    ui->ip_mk->hide();
    ui->ok_mk->hide();
    ui->ca_mk->hide();
}


//*************************************************************
//Thay đổi tình trạng

void Information::on_td_tt_clicked()
{
    ui->ip_tt->show();
    ui->ok_tt->show();
    ui->ca_tt->show();
}

//*********************************
void Information::on_ok_tt_clicked()
{
    if (ui->ip_tt->text()=="")
    {
        QMessageBox::critical(this,"Tình trạng","Tình trạng trống");
        return;
    }

    QSqlQuery query(0,db);
    QString tdn=ui->op_tdn->text();

    query.prepare("update account set status = :newstt where account = :tdn;");
    query.bindValue(":newstt",ui->ip_tt->text());
    query.bindValue(":tdn",tdn);
    bool ms=query.exec();

    if (ms)
    {
        QMessageBox::about(this,"Cập nhật","Cập nhật thành công");
    }
    else
    {
        QMessageBox::about(this,"Cập nhật","Cập nhật thất bại");
    }

    ui->ip_tt->setText("");
    ui->ip_tt->hide();
    ui->ok_tt->hide();
    ui->ca_tt->hide();
    load(tdn,"reader");

}

//*********************************
void Information::on_ca_tt_clicked()
{
    ui->ip_tt->setText("");
    ui->ip_tt->hide();
    ui->ok_tt->hide();
    ui->ca_tt->hide();
}


//*************************************************************
//Thay đổi tên

void Information::on_td_hvt_clicked()
{
    ui->ip_hvt->show();
    ui->ok_hvt->show();
    ui->ca_hvt->show();
}

//*********************************
void Information::on_ok_hvt_clicked()
{
    if (ui->ip_hvt->text()=="")
    {
        QMessageBox::critical(this,"Họ và tên","Họ và tên trống");
        return;
    }

    QSqlQuery query(0,db);
    QString tdn=ui->op_tdn->text();

    query.prepare("update account set user_name = :newname where account = :tdn;");
    query.bindValue(":newname",ui->ip_hvt->text());
    query.bindValue(":tdn",tdn);
    bool ms=query.exec();

    if (ms)
    {
        QMessageBox::about(this,"Cập nhật","Cập nhật thành công");
    }
    else
    {
        QMessageBox::about(this,"Cập nhật","Cập nhật thất bại");
    }

    ui->ip_hvt->setText("");
    ui->ip_hvt->hide();
    ui->ok_hvt->hide();
    ui->ca_hvt->hide();
    load(tdn,"reader");

}

//*********************************
void Information::on_ca_hvt_clicked()
{
    ui->ip_hvt->setText("");
    ui->ip_hvt->hide();
    ui->ok_hvt->hide();
    ui->ca_hvt->hide();
}

//*************************************************************
//Thay đổi chứng minh nhân dân

void Information::on_td_cmnd_clicked()
{
    ui->ip_cmnd->show();
    ui->ok_cmnd->show();
    ui->ca_cmnd->show();
}

//*********************************
void Information::on_ok_cmnd_clicked()
{
    if (ui->ip_cmnd->text()=="")
    {
        QMessageBox::critical(this,"CMND","CMND trống");
        return;
    }

    QSqlQuery query(0,db);
    QString tdn=ui->op_tdn->text();

    query.prepare("update account set identity_id = :new where account = :tdn;");
    query.bindValue(":new",ui->ip_cmnd->text());
    query.bindValue(":tdn",tdn);
    bool ms=query.exec();

    if (ms)
    {
        QMessageBox::about(this,"Cập nhật","Cập nhật thành công");
    }
    else
    {
        QMessageBox::about(this,"Cập nhật","Cập nhật thất bại");
    }

    ui->ip_cmnd->setText("");
    ui->ip_cmnd->hide();
    ui->ok_cmnd->hide();
    ui->ca_cmnd->hide();
    load(tdn,"reader");

}

//*********************************
void Information::on_ca_cmnd_clicked()
{
    ui->ip_cmnd->setText("");
    ui->ip_cmnd->hide();
    ui->ok_cmnd->hide();
    ui->ca_cmnd->hide();
}


//*************************************************************
//Thay đổi giới tính

void Information::on_td_gt_clicked()
{
    ui->ip_gt_nam->show();
    ui->ip_gt_nu->show();
    ui->ok_gt->show();
    ui->ca_gt->show();
}

//*********************************
void Information::on_ok_gt_clicked()
{


    QSqlQuery query(0,db);
    QString tdn=ui->op_tdn->text();

    query.prepare("update account set sex = :new where account = :tdn;");
    query.bindValue(":new",(ui->ip_gt_nam->isChecked()? "Nam" : "Nữ"));
    query.bindValue(":tdn",tdn);
    bool ms=query.exec();

    if (ms)
    {
        QMessageBox::about(this,"Cập nhật","Cập nhật thành công");
    }
    else
    {
        QMessageBox::about(this,"Cập nhật","Cập nhật thất bại");
    }

    ui->ip_gt_nam->hide();
    ui->ip_gt_nu->hide();
    ui->ok_gt->hide();
    ui->ca_gt->hide();
    load(tdn,"reader");

}

//*********************************
void Information::on_ca_gt_clicked()
{
    ui->ip_gt_nam->hide();
    ui->ip_gt_nu->hide();
    ui->ok_gt->hide();
    ui->ca_gt->hide();
}

//*************************************************************
//Thay đổi ngày sinh

void Information::on_td_ns_clicked()
{
    ui->ip_ns_ngay->show();
    ui->ip_ns_thang->show();
    ui->ip_ns_nam->show();
    ui->ok_ns->show();
    ui->ca_ns->show();
}

//*********************************
void Information::on_ok_ns_clicked()
{

    QSqlQuery query(0,db);
    QString tdn=ui->op_tdn->text();

    query.prepare("update account set birthdate = :new where account = :tdn;");
    query.bindValue(":new",ui->ip_ns_ngay->text() +"/"+ ui->ip_ns_thang->text() +"/"+ ui->ip_ns_nam->text());
    query.bindValue(":tdn",tdn);
    bool ms=query.exec();

    if (ms)
    {
        QMessageBox::about(this,"Cập nhật","Cập nhật thành công");
    }
    else
    {
        QMessageBox::about(this,"Cập nhật","Cập nhật thất bại");
    }

    ui->ip_ns_ngay->hide();
    ui->ip_ns_thang->hide();
    ui->ip_ns_nam->hide();
    ui->ok_ns->hide();
    ui->ca_ns->hide();
    load(tdn,"reader");

}

//*********************************
void Information::on_ca_ns_clicked()
{
    ui->ip_ns_ngay->hide();
    ui->ip_ns_thang->hide();
    ui->ip_ns_nam->hide();
    ui->ok_ns->hide();
    ui->ca_ns->hide();
}

//*************************************************************
//Thay đổi nghề nghiệp

void Information::on_td_nn_clicked()
{
    ui->ip_nn->show();
    ui->ok_nn->show();
    ui->ca_nn->show();
}

//*********************************
void Information::on_ok_nn_clicked()
{
    if (ui->ip_nn->text()=="")
    {
        QMessageBox::critical(this,"Nghề nghiệp","Nghề nghiệp trống");
        return;
    }

    QSqlQuery query(0,db);
    QString tdn=ui->op_tdn->text();

    query.prepare("update account set job = :new where account = :tdn;");
    query.bindValue(":new",ui->ip_nn->text());
    query.bindValue(":tdn",tdn);
    bool ms=query.exec();

    if (ms)
    {
        QMessageBox::about(this,"Cập nhật","Cập nhật thành công");
    }
    else
    {
        QMessageBox::about(this,"Cập nhật","Cập nhật thất bại");
    }

    ui->ip_nn->setText("");
    ui->ip_nn->hide();
    ui->ok_nn->hide();
    ui->ca_nn->hide();
    load(tdn,"reader");

}

//*********************************
void Information::on_ca_nn_clicked()
{
    ui->ip_nn->setText("");
    ui->ip_nn->hide();
    ui->ok_nn->hide();
    ui->ca_nn->hide();
}

//*************************************************************
//Thay đổi email

void Information::on_td_em_clicked()
{
    ui->ip_em->show();
    ui->ok_em->show();
    ui->ca_em->show();
}

//*********************************
void Information::on_ok_em_clicked()
{
    if (ui->ip_em->text()=="")
    {
        QMessageBox::critical(this,"Email","Email trống");
        return;
    }

    QSqlQuery query(0,db);
    QString tdn=ui->op_tdn->text();

    query.prepare("update account set email = :new where account = :tdn;");
    query.bindValue(":new",ui->ip_em->text());
    query.bindValue(":tdn",tdn);
    bool ms=query.exec();

    if (ms)
    {
        QMessageBox::about(this,"Cập nhật","Cập nhật thành công");
    }
    else
    {
        QMessageBox::about(this,"Cập nhật","Cập nhật thất bại");
    }

    ui->ip_em->setText("");
    ui->ip_em->hide();
    ui->ok_em->hide();
    ui->ca_em->hide();
    load(tdn,"reader");

}

//*********************************
void Information::on_ca_em_clicked()
{
    ui->ip_em->setText("");
    ui->ip_em->hide();
    ui->ok_em->hide();
    ui->ca_em->hide();
}

void Information::on_ca_clicked()
{
    this->close();
}


//*************************************************************
//Thay đổi vai trò

void Information::on_td_vt_clicked()
{
    ui->ip_vt_librarian->show();
    ui->ip_vt_manager->show();
    ui->ip_vt_reader->show();
    ui->ok_vt->show();
    ui->ca_vt->show();
}

//*********************************
void Information::on_ok_vt_clicked()
{
    QSqlQuery query(0,db);
    QString tdn=ui->op_tdn->text();

    query.prepare("delete from role where account = :tdn;");
    query.bindValue(":tdn",tdn);
    query.exec();

    bool ms;

    if (ui->ip_vt_librarian->isChecked())
    {
        query.prepare("insert into role values(:tdn, :role);");
        query.bindValue(":tdn",tdn);
        query.bindValue(":role","Librarian");
        ms=query.exec();
    }

    if (ui->ip_vt_manager->isChecked())
    {
        query.prepare("insert into role values(:tdn, :role);");
        query.bindValue(":tdn",tdn);
        query.bindValue(":role","Manager");
        ms=query.exec();
    }

    if (ui->ip_vt_reader->isChecked())
    {
        query.prepare("insert into role values(:tdn, :role);");
        query.bindValue(":tdn",tdn);
        query.bindValue(":role","Reader");
        ms=query.exec();
    }

    if (ms)
    {
        QMessageBox::about(this,"Cập nhật","Cập nhật thành công");
    }
    else
    {
        QMessageBox::about(this,"Cập nhật","Cập nhật thất bại");
    }

    ui->ip_vt_librarian->setChecked(0);
    ui->ip_vt_manager->setChecked(0);
    ui->ip_vt_reader->setChecked(0);
    ui->ip_vt_librarian->hide();
    ui->ip_vt_manager->hide();
    ui->ip_vt_reader->hide();
    ui->ok_vt->hide();
    ui->ca_vt->hide();
    load(tdn,"reader");

}

//*********************************
void Information::on_ca_vt_clicked()
{
    ui->ip_vt_librarian->setChecked(0);
    ui->ip_vt_manager->setChecked(0);
    ui->ip_vt_reader->setChecked(0);
    ui->ip_vt_librarian->hide();
    ui->ip_vt_manager->hide();
    ui->ip_vt_reader->hide();
    ui->ok_vt->hide();
    ui->ca_vt->hide();
}
