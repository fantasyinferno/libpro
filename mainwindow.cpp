#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QSqlTableModel>
#include "bookdelegate.h"
#include <QDataWidgetMapper>
#include <QSqlQuery>
#include <qDebug>
#include <QDate>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>
#include <QByteArray>
#include <QSqlRelation>
#include <QSqlRelationalTableModel>
#include "addbook.h"
#include "finedialog.h"

MainWindow::MainWindow(QWidget *parent, QSqlDatabase database) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initializeGUILogic(database);
    user_id = 0;
    user = "";
}

MainWindow::~MainWindow()
{
    delete ui;
}
QSqlDatabase MainWindow::getDatabase() {
    return db;
}
QString MainWindow::getUser() {
    return user;
}

void MainWindow::on_thanhTimKiem_returnPressed()
{
    QString keyword = ui->thanhTimKiem->text();
    if (!keyword.isEmpty()) {
        QString queryString = QString("book MATCH 'title:%1 OR author:%1 OR description:%1 OR book_id:%1'").arg(keyword);
        model->setFilter(queryString);
    }
}
void MainWindow::initializeDatabase(QSqlDatabase database = QSqlDatabase()) {
    db = database;
}

void MainWindow::initializeTable()
{
    // Thiết lập model
    model = new QSqlTableModel(this, db);
    model->setTable("book");
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("ISBN"));
    model->setHeaderData(1, Qt::Horizontal, tr("Bìa sách"));
    model->setHeaderData(2, Qt::Horizontal, tr("Tựa đề"));
    model->setHeaderData(3, Qt::Horizontal, tr("Tác giả"));
    model->setHeaderData(4, Qt::Horizontal, tr("Số lượng trang"));
    model->setHeaderData(5, Qt::Horizontal, tr("Năm sản xuất"));
    model->setHeaderData(6, Qt::Horizontal, tr("Nội dung"));
    model->setHeaderData(7, Qt::Horizontal, tr("Trạng thái"));
    // Cột thứ 8!
    model->insertColumn(8);
    model->setHeaderData(8, Qt::Horizontal, tr("Tùy chọn"));
    // Thiết lập delegate
    BookDelegate *bd = new BookDelegate(ui->danhMucSach);
    // Thiết lập View
    ui->danhMucSach->setModel(model);
    ui->danhMucSach->setItemDelegate(bd);
    ui->danhMucSach->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->danhMucSach->horizontalHeader()->setVisible(true);
    ui->danhMucSach->setColumnWidth(1, 60);
    ui->danhMucSach->setColumnWidth(2, 400);
    ui->danhMucSach->setColumnWidth(3, 400);
    ui->danhMucSach->setColumnHidden(6, true);
    bookMapper = new QDataWidgetMapper(this);
    bookMapper->setModel(model);
    bookMapper->setItemDelegate(new BookDelegate(this));
    bookMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    bookMapper->addMapping(ui->maSach, model->fieldIndex("book_id"));
    bookMapper->addMapping(ui->biaSach, model->fieldIndex("cover"));
    bookMapper->addMapping(ui->noiDungSach, model->fieldIndex("description"));
    bookMapper->addMapping(ui->tuaDe, model->fieldIndex("title"));
    bookMapper->addMapping(ui->tacGia, model->fieldIndex("author"));
    bookMapper->addMapping(ui->namSanXuat, model->fieldIndex("year"));
    bookMapper->addMapping(ui->biaSach, model->fieldIndex("cover"));
    connect(ui->danhMucSach->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), bookMapper, SLOT(setCurrentModelIndex(QModelIndex)));
    connect(ui->danhMucSach->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), ui->hienThiSach, SLOT(show()));
    ui->hienThiSach->hide();
}
void MainWindow::initializeQuotes() {
    QSqlQuery quoteQuery(db);
    quoteQuery.exec("SELECT * FROM quotes WHERE rowid >= (abs(random()) % (SELECT MAX(rowid) FROM quotes))");
    quoteQuery.next();
    QString quote = quoteQuery.value(0).toString();
    QString author = quoteQuery.value(1).toString();
    ui->quotesLabel->setText("'" + quote + "'\n - " + author);
}

void MainWindow::initializeButtons()
{
    ui->dangXuatButton->hide();
    ui->thayDoiSachButton->hide();
    ui->themSachButton->hide();
    ui->chapNhanSachButton->hide();
    ui->huySachButton->hide();
}

void MainWindow::enableLibrarianButtons(bool chapThuan, bool tuChoi, bool phat, bool xacNhan)
{
    ui->chapThuanButton->setEnabled(chapThuan);
    ui->tuChoiButton->setEnabled(tuChoi);
    ui->phatButton->setEnabled(phat);
    ui->xacNhanSachDaTraButton->setEnabled(xacNhan);
}
void MainWindow::initializeGUILogic(QSqlDatabase database) {
    initializeDatabase(database);
    initializeTable();
    initializeQuotes();
    initializeButtons();
    connect(ui->timKiemButton, SIGNAL(clicked()), this, SLOT(on_thanhTimKiem_returnPressed()));

    ui->username->setEnabled(false);
    ui->toolBox->setItemEnabled(0, true);
    ui->toolBox->setItemEnabled(1, false);
    ui->toolBox->setItemEnabled(2, false);
}

void MainWindow::on_dangXuatButton_clicked()
{
    // Bỏ trống user và user_id
    user = "";
    user_id = 0;
    // Thay đổi giao diện về lại ban đầu
    ui->username->setText("");
    ui->username->setEnabled(false);
    ui->dangXuatButton->hide();
    ui->dangKyButton->show();
    ui->dangNhapButton->show();
    ui->avatarIcon->clear();
    ui->toolBox->setItemEnabled(0, true);
    // Nếu có role thủ thư thì rollback những gì của thủ thư
    if (rolesList.contains(2)) {
        // Làm ngược lại với on_rolesLoaded()
        ui->toolBox->setItemEnabled(1, false);
        ui->danhMucSach->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->danhMucSach->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->thayDoiSachButton->hide();
        ui->themSachButton->hide();
        ui->tabWidget->setCurrentIndex(0);
        ui->muonSachTable->setModel(0);
        ui->dangMuonTable->setModel(0);
        ui->matSachTable->setModel(0);
        delete requestBookModel;
    }
    ui->toolBox->setItemEnabled(2, false);
    emit dangXuat();
}

void MainWindow::on_dangNhapButton_clicked()
{
    emit formRequest(0);
}

void MainWindow::on_dangKyButton_clicked()
{
    emit formRequest(1);
}
void MainWindow::on_username_clicked() {
    emit informationRequest();
}

void MainWindow::on_dangNhapThanhCong(int id, QString username) {
    user = username;
    user_id = id;
    ui->dangKyButton->hide();
    ui->dangNhapButton->hide();
    ui->dangXuatButton->show();
    ui->username->setText(user);
    ui->username->setEnabled(true);
}

void MainWindow::on_avatarChanged(const QPixmap* pixmap) {
    ui->avatarIcon->setPixmap(*pixmap);
}

void MainWindow::on_actionAbout_LIBPRO_triggered() {
    emit aboutTriggered();
}

void MainWindow::on_rolesLoaded(QList<int>& list)
{
    rolesList = list;
    for (int p: (rolesList)) {
        ui->toolBox->setItemEnabled(p - 1, true);
    }
    if (rolesList.contains(2)) {
        // Có thể sửa sách
        ui->thayDoiSachButton->show();
        ui->themSachButton->show();

        // Có thể chấp thuận yêu cầu sách
        requestBookModel = new QSqlRelationalTableModel(this, db);
        requestBookModel->setEditStrategy(QSqlTableModel::OnFieldChange);
        requestBookModel->setTable("account_book");
        bookIdIdx = requestBookModel->fieldIndex("book_id");
        accountIdIdx = requestBookModel->fieldIndex("account_id");
        bookStatusIdIdx = requestBookModel->fieldIndex("book_status_id");
        requestBookModel->setRelation(bookIdIdx, QSqlRelation("book", "book_id", "title"));
        requestBookModel->setRelation(accountIdIdx, QSqlRelation("account", "account_id", "account"));
        requestBookModel->setRelation(bookStatusIdIdx, QSqlRelation("book_status", "book_status_id", "book_status"));
        requestBookModel->setHeaderData(bookIdIdx, Qt::Horizontal, tr("Tựa đề"));
        requestBookModel->setHeaderData(accountIdIdx, Qt::Horizontal, tr("Người mượn"));
        requestBookModel->setHeaderData(requestBookModel->fieldIndex("start_date"), Qt::Horizontal, tr("Ngày ký mượn"));
        requestBookModel->setHeaderData(requestBookModel->fieldIndex("due_date"), Qt::Horizontal, tr("Ngày cần trả"));
        requestBookModel->setHeaderData(bookStatusIdIdx, Qt::Horizontal, tr("Trạng thái"));
        requestBookModel->select();
        ui->muonSachTable->setModel(requestBookModel);
        ui->muonSachTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->muonSachTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->muonSachTable->setColumnHidden(requestBookModel->fieldIndex("account_book_id"), true);
        ui->dangMuonTable->setModel(requestBookModel);
        ui->dangMuonTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->dangMuonTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->dangMuonTable->setColumnHidden(requestBookModel->fieldIndex("account_book_id"), true);
        ui->matSachTable->setModel(requestBookModel);
        ui->matSachTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->matSachTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->matSachTable->setColumnHidden(requestBookModel->fieldIndex("account_book_id"), true);
        on_tabWidget_currentChanged(ui->tabWidget->currentIndex());
    }
}

void MainWindow::on_muonButton_clicked()
{
    if (user_id < 0) {
        QModelIndexList list = ui->danhMucSach->selectionModel()->selectedRows(0);
        if (!list.isEmpty()) {
            QSqlQuery query(0, db);
            query.prepare("INSERT INTO account_book(account_id, book_id, book_status_id) VALUES(?, ?, ?)");
            bool ok = false;
            for (QModelIndex index: list) {
                query.addBindValue(user_id);
                query.addBindValue(index.data().toString());
                query.addBindValue(1);
                if (!(ok = query.exec())) {
                    qDebug() << query.lastError();
                }
            }
            if (ok) {
                QMessageBox::information(this, "Mượn sách", "Bạn đã gửi yêu cầu mượn sách!");
            } else {
                QMessageBox::warning(this, "Mượn sách", "Đã có lỗi xảy ra!");
            }
        }
    } else {
        on_dangNhapButton_clicked();
    }
}

void MainWindow::on_chapThuanButton_clicked()
{
    QModelIndexList list = ui->muonSachTable->selectionModel()->selectedRows(requestBookModel->fieldIndex("book_status"));
    if (!list.isEmpty()) {
        for (QModelIndex &i: list) {
            requestBookModel->setData(i, 2);
            QDate today = QDate::currentDate();
            requestBookModel->setData(i.sibling(i.row(), requestBookModel->fieldIndex("start_date")), today);
            requestBookModel->setData(i.sibling(i.row(), requestBookModel->fieldIndex("due_date")), today.addDays(15));
        }
        requestBookModel->select();
        QMessageBox::information(this, "Thành công!", "Bạn đã cho phép độc giả mượn sách!");
        // Gửi tin nhắn đến thành viên
    }
}

void MainWindow::on_tuChoiButton_clicked()
{
    QModelIndexList list = ui->muonSachTable->selectionModel()->selectedRows(requestBookModel->fieldIndex("book_status"));
    if (!list.isEmpty()) {
        for (QModelIndex &i: list) {
            requestBookModel->removeRow(i.row());
        }
        requestBookModel->select();
        QMessageBox::information(this, "Thành công!", "Bạn đã từ chối yêu cầu của độc giả!");
        // Gửi tin nhắn đến thành viên
    }
}

void MainWindow::on_phatButton_clicked()
{
    QModelIndexList list;
    if (ui->tabWidget->currentIndex() == 1) {
        list = ui->dangMuonTable->selectionModel()->selectedRows(bookStatusIdIdx);
    } else {
        list = ui->matSachTable->selectionModel()->selectedRows(bookStatusIdIdx);
    }
    if (!list.isEmpty()) {
        FineDialog *fineDialog = new FineDialog(this, db, list);
        fineDialog->setWindowTitle("Phạt");
        // Thuộc tính này giúp fineDialog tự giải phóng bộ nhớ sau khi tắt dialog
        fineDialog->setAttribute(Qt::WA_DeleteOnClose);
        fineDialog->show();
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index == 0) {
        // Chờ duyệt
        requestBookModel->setFilter("book_status = 'Chờ duyệt'");
        enableLibrarianButtons(1, 1, 0, 0);
    } else if (index == 1) {
        // Sách đang mượn
        on_hanNop_currentIndexChanged(ui->hanNop->currentIndex());
    } else if (index == 2) {
        // Sách quá hạn
        requestBookModel->setFilter("book_status = 'Bị mất'");
        enableLibrarianButtons(0, 0, 1, 0);
    }
}

void MainWindow::on_hanNop_currentIndexChanged(int index)
{
    if (index == 0) {
        // Sách trong kỳ hạn
        requestBookModel->setFilter("book_status = 'Đang mượn' AND julianday(due_date) - julianday(start_date) <= 15");
        enableLibrarianButtons(0, 0, 0, 1);
    } else {
        // Sách quá hạn
        requestBookModel->setFilter("book_status = 'Đang mượn' AND julianday(due_date) - julianday(start_date) > 15");
        enableLibrarianButtons(0, 0, 1, 1);
    }
}

void MainWindow::on_xacNhanSachDaTraButton_clicked()
{
    QModelIndexList list = ui->dangMuonTable->selectionModel()->selectedRows(requestBookModel->fieldIndex("book_status"));
    if (!list.isEmpty()) {
        for (QModelIndex &i: list) {
            requestBookModel->setData(i, 3);
        }
        requestBookModel->select();
        QMessageBox::information(this, "Thành công!", "Bạn đã xác nhận sách đã trả!");
        // Gửi tin nhắn đến thành viên
    }
}

void MainWindow::on_thayDoiSachButton_clicked()
{
    ui->maSach->setReadOnly(false);
    ui->namSanXuat->setReadOnly(false);
    ui->tacGia->setReadOnly(false);
    ui->theLoai->setReadOnly(false);
    ui->tuaDe->setReadOnly(false);
    ui->noiDungSach->setReadOnly(false);
    ui->thayDoiSachButton->hide();
    ui->chapNhanSachButton->show();
    ui->huySachButton->show();
}

void MainWindow::on_chapNhanSachButton_clicked()
{
    bookMapper->submit();
    ui->maSach->setReadOnly(true);
    ui->namSanXuat->setReadOnly(true);
    ui->tacGia->setReadOnly(true);
    ui->theLoai->setReadOnly(true);
    ui->tuaDe->setReadOnly(true);
    ui->noiDungSach->setReadOnly(true);
    ui->thayDoiSachButton->show();
    ui->chapNhanSachButton->hide();
    ui->huySachButton->hide();
}

void MainWindow::on_huySachButton_clicked()
{
    bookMapper->revert();
    ui->maSach->setReadOnly(true);
    ui->namSanXuat->setReadOnly(true);
    ui->tacGia->setReadOnly(true);
    ui->theLoai->setReadOnly(true);
    ui->tuaDe->setReadOnly(true);
    ui->noiDungSach->setReadOnly(true);
    ui->thayDoiSachButton->show();
    ui->chapNhanSachButton->hide();
    ui->huySachButton->hide();
}

void MainWindow::on_themSachButton_clicked()
{
    AddBook *addBook = new AddBook(0, db);
    // Tự động giải phóng bộ nhớ sau khi tắt
    addBook->setAttribute(Qt::WA_DeleteOnClose);
    addBook->show();
    // Cập nhật lại model nếu có sách mới thêm vào
    QObject::connect(addBook, SIGNAL(accepted()), model, SLOT(select()));
}
