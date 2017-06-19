#include "managergui.h"
#include "ui_managergui.h"

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

#include "introform.h"
#include "readergui.h"
#include "bookdelegate.h"

ManagerGUI::ManagerGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManagerGUI)
{
    ui->setupUi(this);
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/pc/Documents/QT/Ass2/Git/libpro/libpro.db");
    db.open();

    initializeTable();
}

ManagerGUI::~ManagerGUI()
{
    delete ui;
}

void ManagerGUI::initializeTable()
{
    // Thiết lập model
    model = new QSqlRelationalTableModel(new QObject(), db);
    model->setTable("account");
    int genderIdx = model->fieldIndex("gender_id");
    int statusIdx = model->fieldIndex("status_id");
    model->setRelation(genderIdx, QSqlRelation("gender", "gender_id", "gender"));
    model->setRelation(statusIdx, QSqlRelation("status", "status_id", "status"));
    model->select();

    QSqlTableModel *relModelGender = model->relationModel(genderIdx);
    QSqlTableModel *relModelStatus = model->relationModel(statusIdx);
    ui->gioiTinh->setModel(relModelGender);
    ui->gioiTinh->setModelColumn(relModelGender->fieldIndex("gender"));
    ui->tinhTrang->setModel(relModelStatus);
    ui->tinhTrang->setModelColumn(relModelStatus->fieldIndex("status"));

    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Tên đăng nhập"));
    model->setHeaderData(2, Qt::Horizontal, tr("Mật khẩu"));
    model->setHeaderData(3, Qt::Horizontal, tr("Tình trạng"));
    model->setHeaderData(4, Qt::Horizontal, tr("Họ và tên"));
    model->setHeaderData(5, Qt::Horizontal, tr("CMND"));
    model->setHeaderData(6, Qt::Horizontal, tr("Giới tính"));
    model->setHeaderData(7, Qt::Horizontal, tr("Ngày sinh"));
    model->setHeaderData(8, Qt::Horizontal, tr("Email"));
    model->setHeaderData(9, Qt::Horizontal, tr("Nghề nghiệp"));
    // Cột thứ 10!
    model->insertColumn(10);
    model->setHeaderData(10, Qt::Horizontal, tr("Tùy chọn"));
    // Thiết lập View
    ui->danhSachThanhVien->setModel(model);
    ui->danhSachThanhVien->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->danhSachThanhVien->horizontalHeader()->setVisible(true);
    ui->danhSachThanhVien->setColumnWidth(0, 30);
    ui->danhSachThanhVien->setColumnWidth(1, 150);
    ui->danhSachThanhVien->setColumnWidth(2, 120);
    ui->danhSachThanhVien->setColumnWidth(3, 80);
    ui->danhSachThanhVien->setColumnWidth(4, 150);
    ui->danhSachThanhVien->setColumnWidth(5, 100);
    ui->danhSachThanhVien->setColumnWidth(6, 80);
    ui->danhSachThanhVien->setColumnWidth(7, 100);
    ui->danhSachThanhVien->setColumnWidth(8, 150);
    ui->danhSachThanhVien->setColumnWidth(9, 150);
    ui->danhSachThanhVien->setColumnHidden(2, true);
    QObject::connect(ui->danhSachThanhVien->selectionModel(), SIGNAL(selectionChanged( const QItemSelection &, const QItemSelection &)), SLOT(on_selectionChanged(const QItemSelection &, const QItemSelection &)));
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->addMapping(ui->id, model->fieldIndex("account_id"));
    mapper->addMapping(ui->tenDangNhap, model->fieldIndex("account"));
    mapper->addMapping(ui->matKhau, model->fieldIndex("password"));
    mapper->addMapping(ui->tinhTrang, model->fieldIndex("status_id"));
    mapper->addMapping(ui->hoVaTen, model->fieldIndex("fullname"));
    mapper->addMapping(ui->CMND, model->fieldIndex("identity_number"));
    mapper->addMapping(ui->gioiTinh, genderIdx);
    mapper->addMapping(ui->tinhTrang, statusIdx);
    mapper->addMapping(ui->email, model->fieldIndex("email"));
    mapper->addMapping(ui->congViec, model->fieldIndex("job"));
    mapper->addMapping(ui->ngaySinh, model->fieldIndex("birthdate"));
    mapper->toFirst();

    checkVt();

    QObject::connect(ui->danhSachThanhVien->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), mapper, SLOT(setCurrentModelIndex(QModelIndex)));
    QObject::connect(ui->danhSachThanhVien->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(checkVt()));

    ui->id->setEnabled(false);
    ui->tenDangNhap->setEnabled(false);

    QStringList gioitinh=(QStringList()<<"Tất cả"<<"Nam"<<"Nữ");
    QStringList tinhtrang=(QStringList()<<"Tất cả"<<"Hoạt động"<<"Bị khóa");
    ui->combo_gioitinh->addItems(gioitinh);
    ui->combo_tinhtrang->addItems(tinhtrang);

}


void ManagerGUI::checkVt() {

    ui->reader->setChecked(false);
    ui->manager->setChecked(false);
    ui->librarian->setChecked(false);

    QSqlQuery query(0, db);

    query.prepare("select account_id from account where account = :tdn;");
    query.bindValue(":tdn", ui->tenDangNhap->text());
    query.exec();
    query.next();
//    int id = query.value(0).toInt();

    query.prepare("SELECT * FROM account_role WHERE account_id = :id");
    query.bindValue(":id", ui->id->text());
    query.exec();

    while (query.next()) {
        int role = query.value(1).toInt();
        if (role == 1) {
            ui->reader->setChecked(true);
        } else if (role == 2) {
            ui->librarian->setChecked(true);
        } else{
            ui->manager->setChecked(true);
        }
    }
}

void ManagerGUI::submitVt()
{
    QSqlQuery query(0,db);

    query.prepare("select account_id from account where account = :tdn;");
    query.bindValue(":tdn", ui->tenDangNhap->text());
    query.exec();
    query.next();
    int id = query.value(0).toInt();

    query.prepare("delete from account_role where account_id = :id;");
    query.bindValue(":id", id);
    query.exec();
    bool ms;
    // TODO: replace account's name with it's id. Connect checkboxes to the model.
    if (ui->librarian->isChecked())
    {
        query.prepare("insert into account_role values(:id, :role);");
        query.bindValue(":id",id);
        query.bindValue(":role", 2);
        ms=query.exec();
    }

    if (ui->manager->isChecked())
    {
        query.prepare("insert into account_role values(:id, :role);");
        query.bindValue(":id",id);
        query.bindValue(":role", 3);
        ms=query.exec();
    }

    if (ui->reader->isChecked())
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

void ManagerGUI::on_thayDoiButton_clicked()
{
    mapper->submit();
    submitVt();

    QSqlQuery query(0,db);
    qDebug()<<ui->tinhTrang->currentText();
    qDebug()<<ui->id->text();

    query.prepare("update account set status_id=:tt where account_id=:id");
    query.bindValue(":tt",(ui->tinhTrang->currentText()=="Hoạt động")? "1" : "2");
    query.bindValue(":id",ui->id->text());
    qDebug()<<query.exec();

    query.prepare("update account set gender_id=:gt where account_id=:id");
    query.bindValue(":gt",ui->gioiTinh->currentText()=="Nam"? "1" : "2");
    query.bindValue(":id",ui->id->text());
    qDebug()<<query.exec();
}

void ManagerGUI::on_xoaButton_clicked()
{
    QSqlQuery query(0, db);
    query.prepare("delete from account_role where account_id= :id");
    query.bindValue(":id", ui->id->text());
    query.exec();

    query.prepare("delete from account where account_id= :id");
    query.bindValue(":id", ui->id->text());
    bool ms=query.exec();
    if (ms)
        QMessageBox::about(this,"Xóa tài khoản","Xóa tài khoản thành công!");
    else
        QMessageBox::about(this,"Xóa tài khoản","Xóa tài khoản thất bại!");
}


void ManagerGUI::on_thanhTimKiem_returnPressed()
{


//    QString type = ui->luaChon->text();
    QString keyword = ui->thanhTimKiem->text();

    QString s="";

    if (ui->f_id->isChecked())
    {
        qDebug()<<"id='"+keyword+"'";
        model->setFilter("account_id="+keyword);
        return;
    }

    if (ui->f_tendangnhap->isChecked())
    {
        s=s+((s=="")? "(" : " OR ")+"account LIKE " + ((keyword=="")? "'%'" : "'%%1%'");
    }

    if (ui->f_hovaten->isChecked())
    {
        s=s+((s=="")? "(" : " OR ")+"fullname LIKE " + ((keyword=="")? "'%'" : "'%%1%'");
    }

    if (ui->f_cmnd->isChecked())
    {
        s=s+((s=="")? "(" : " OR ")+"identity_number LIKE " + ((keyword=="")? "'%'" : "'%%1%'");
    }

    s=s+((s=="")? "" : ") ");

    if (ui->combo_gioitinh->currentText()!="Tất cả")
    {
        s=s+((s=="")? "" : " AND ")+"gender='"+ui->combo_gioitinh->currentText()+"'";
    }

    if (ui->combo_tinhtrang->currentText()!="Tất cả")
    {
        s=s+((s=="")? "" : " AND ")+"status='"+ui->combo_tinhtrang->currentText()+"'";
    }
    if (s=="")
    {
        model->setFilter("");
        return;
    }

    if (keyword!="")
        s=s.arg(keyword);
    qDebug()<<s;
    model->setFilter(s);

}

void ManagerGUI::on_timKiemButton_clicked()
{
    on_thanhTimKiem_returnPressed();
}
