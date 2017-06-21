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

Information::Information(QWidget *parent, QSqlDatabase database) :
    QDialog(parent),
    ui(new Ui::Information)
{
    ui->setupUi(this);
    this->setModal(true);
    db = database;
    enableEdit(false);
    // Model cho thông tin cá nhân
    model = new QSqlRelationalTableModel(this, db);
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

    // Thiết lập model xử lý lược sử mượn sách
    bookModel = new QSqlRelationalTableModel(this);
    bookModel->setTable("account_book");
    bookModel->setEditStrategy(QSqlTableModel::OnFieldChange);
    bookModel->setRelation(bookModel->fieldIndex("book_id"), QSqlRelation("book", "book_id", "title"));
    bookModel->setRelation(bookModel->fieldIndex("book_status_id"), QSqlRelation("book_status", "book_status_id", "book_status"));
    bookModel->setHeaderData(2, Qt::Horizontal, "Tựa đề");
    bookModel->setHeaderData(3, Qt::Horizontal,  "Ngày mượn");
    bookModel->setHeaderData(4, Qt::Horizontal, "Ngày hết hạn");
    bookModel->setHeaderData(5, Qt::Horizontal, "Trạng thái");
}
void Information::on_informationRequest() {
    this->show();
    on_tabWidget_currentChanged(ui->tabWidget->currentIndex());
}

void Information::load(QString tendangnhap, QString vaitro)
{

    ui->ip_vt_librarian->setEnabled(false);
    ui->ip_vt_manager->setEnabled(false);
    ui->ip_vt_reader->setEnabled(false);
    ui->ip_tt->setEnabled(false);

    enableEdit(false);

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

int Information::getBorrowedNumOfBook()
{
    bookModel->setFilter("(book_status = 'Đang mượn' OR book_status = 'Chờ duyệt') AND account_id = " + user_id);
    return bookModel->rowCount();
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
//    ui->ip_tt->setEnabled(enabled);
//    ui->ip_vt_librarian->setEnabled(enabled);
//    ui->ip_vt_manager->setEnabled(enabled);
//    ui->ip_vt_reader->setEnabled(enabled);
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
    // Lưu tên đăng nhập và id mỗi lần đăng nhập
    user = username;
    ui->username->setText(user);
    user_id = id;
    // Model chỉ hiển thị thông tin của người đăng nhập
    model->setFilter(QString("account_id = '%1'").arg(user_id)); // SQL Injection Alert!
    model->select();
    // Gọi mapper
    mapper->toFirst();
    // Gọi hàm kiểm tra vai trò
    checkVt();
    // Model chỉ hiển thị lược sử sách của người đăng nhập
    bookModel->select();
    // Đưa model cho 3 bảng
    ui->sachDangMuon->setModel(bookModel);
    ui->sachDangMuon->setItemDelegate(new QSqlRelationalDelegate(this));
    ui->sachDangMuon->setColumnHidden(bookModel->fieldIndex("account_id"), true);
    ui->sachDangMuon->setColumnHidden(bookModel->fieldIndex("account_book_id"), true);
    ui->sachChoDuyet->setModel(bookModel);
    ui->sachChoDuyet->setItemDelegate(new QSqlRelationalDelegate(this));
    ui->sachChoDuyet->setColumnHidden(bookModel->fieldIndex("account_id"), true);
    ui->sachChoDuyet->setColumnHidden(bookModel->fieldIndex("account_book_id"), true);
    ui->sachDaMuon->setModel(bookModel);
    ui->sachDaMuon->setItemDelegate(new QSqlRelationalDelegate(this));
    ui->sachDaMuon->setColumnHidden(bookModel->fieldIndex("account_id"), true);
    ui->sachDaMuon->setColumnHidden(bookModel->fieldIndex("account_book_id"), true);
}
void Information::on_huyButton_clicked()
{
    enableEdit(false);
    mapper->revert();
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
    // Để tab mặc định là 0
    ui->tabWidget->setCurrentIndex(0);
    // Bỏ trống mô hình
    model->setFilter("0");
    bookModel->setFilter("0");
    // Nhập lại mappers
    mapper->toFirst();
    // Xóa danh sách vai trò
    rolesList.clear();
    // Ẩn cửa sổ nếu cần thiết
    this->hide();
}

void Information::on_matSachButton_clicked()
{
     QModelIndexList list = ui->sachDangMuon->selectionModel()->selectedRows(bookModel->fieldIndex("book_status"));
     if (!list.isEmpty()) {
        if (QMessageBox::question(this, "Báo cáo mất sách", "Bạn làm mất các sách đã chọn?") == QMessageBox::Yes) {
            for (QModelIndex index: list) {
                bookModel->setData(index, 4);
            }
            bookModel->select();
            QMessageBox::information(this, "Cập nhật thành công!", "Chúng tôi đã báo cáo hành vi làm mất sách của bạn. Vui lòng chờ xử lý từ thủ thư.");
        }
     }
}

void Information::on_tabWidget_currentChanged(int index)
{
    if (index == 0) {
        bookModel->setFilter(QString("book_status = 'Đang mượn' AND account_id = %1").arg(user_id));
    }
    else if (index == 1) {
        bookModel->setFilter(QString("book_status = 'Chờ duyệt' AND account_id = %1").arg(user_id));
    } else {
        bookModel->setFilter(QString("(book_status = 'Đã trả' OR book_status = 'Bị mất') AND account_id = %1").arg(user_id));
    }
    bookModel->select();
}
