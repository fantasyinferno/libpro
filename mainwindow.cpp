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
#include <QFileDialog>
#include "addbook.h"
#include "finedialog.h"
#include "accountdelegate.h"
#include "inbox.h"

MainWindow::MainWindow(QWidget *parent, QSqlDatabase database) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    introForm = new IntroForm(this, db);
    information = new Information(this, db);
    inbox = new Inbox(this, db);
    about = new About(this);
    introForm->setWindowTitle(tr("Đăng nhập/Đăng ký"));
    information->setWindowTitle(tr("Thông tin cá nhân"));
    inbox->setWindowTitle(tr("Hộp thư"));
    about->setWindowTitle(tr("Về LIBPRO"));
    QObject::connect(introForm, SIGNAL(dangNhapThanhCong(int, QString)), this, SLOT(on_dangNhapThanhCong(int, QString)));
    QObject::connect(introForm, SIGNAL(dangNhapThanhCong(int, QString)), information, SLOT(on_dangNhapThanhCong(int, QString)));
    QObject::connect(introForm, SIGNAL(dangNhapThanhCong(int, QString)), inbox, SLOT(on_dangNhapThanhCong(int,QString)));
    QObject::connect(information, SIGNAL(avatarChanged(const QPixmap*)), this, SLOT(on_avatarChanged(const QPixmap*)));
    /* TODO: Thay thế 5 cái này bằng slots của buttons */
    QObject::connect(this, SIGNAL(dangXuat()), information, SLOT(on_dangXuat()));
    QObject::connect(this, SIGNAL(dangXuat()), inbox, SLOT(on_dangXuat()));
    QObject::connect(this, SIGNAL(formRequest(int)), introForm, SLOT(on_formRequest(int)));
    QObject::connect(this, SIGNAL(informationRequest()), information, SLOT(on_informationRequest()));
    QObject::connect(this, SIGNAL(inboxRequest()), inbox, SLOT(show()));
    /* */
    QObject::connect(this, SIGNAL(aboutTriggered()), about, SLOT(show()));
    QObject::connect(information, SIGNAL(rolesLoaded(QList<int>&)), this, SLOT(on_rolesLoaded(QList<int>&)));
    QObject::connect(inbox, SIGNAL(tinNhanMoi(int)), this, SLOT(on_tinNhanMoi(int)));
    initializeGUILogic(database);
    user_id = 0;
    user = "";
    // Đặt giới hạn lên các trường thông tin ở quần quản lý người dùng
    QRegExp mkReg(".{10,}");
    QRegExp emReg("\\b[a-z0-9._%+-]+@[a-z0-9.-]+\\.[a-z]{2,4}\\b");
    QRegExp hvtReg("^([^0-9]+)$");
    QRegExp cvReg(".+");
    QRegExp cmndReg("^([0-9]{9}|[0-9]{12})$");
    ui->matKhau->setValidator(new QRegExpValidator(mkReg));
    ui->email->setValidator(new QRegExpValidator(emReg, this));
    ui->hoVaTen->setValidator(new QRegExpValidator(hvtReg, this));
    ui->congViec->setValidator(new QRegExpValidator(cvReg));
    ui->CMND->setValidator(new QRegExpValidator(cmndReg, this));
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
    } else {
        model->setFilter("");
    }
}
void MainWindow::initializeDatabase(QSqlDatabase database = QSqlDatabase()) {
    db = database;
}

void MainWindow::initializeTable()
{
    // Thiết lập model (cho sách)
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
    // Thiết lập delegate (cho sách)
    BookDelegate *bd = new BookDelegate(ui->danhMucSach);
    // Thiết lập View (cho sách)
    ui->danhMucSach->setModel(model);
    ui->danhMucSach->setItemDelegate(bd);
    ui->danhMucSach->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->danhMucSach->horizontalHeader()->setVisible(true);
    ui->danhMucSach->setColumnHidden(6, true);
    ui->danhMucSach->resizeColumnsToContents();
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
    bookMapper->addMapping(ui->tinhTrangSach, model->fieldIndex("status"), "currentIndex");
    connect(ui->danhMucSach->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), bookMapper, SLOT(setCurrentModelIndex(QModelIndex)));
    connect(ui->danhMucSach->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(enableButtonsProperly()));
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
    ui->hopThuButton->hide();
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
    ui->hopThuButton->hide();
    ui->dangXuatButton->hide();
    ui->dangKyButton->show();
    ui->dangNhapButton->show();
    ui->avatarIcon->clear();
    ui->toolBox->setItemEnabled(0, true);
    // Nếu có role thủ thư thì rollback những gì của thủ thư
    if (rolesList.contains(2)) {
        // Làm ngược lại với on_rolesLoaded()
        ui->toolBox->setItemEnabled(1, false);
        ui->danhMucSach->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->thayDoiSachButton->hide();
        ui->themSachButton->hide();
        ui->tabWidget->setCurrentIndex(0);
        ui->muonSachTable->setModel(0);
        ui->dangMuonTable->setModel(0);
        ui->matSachTable->setModel(0);
        delete requestBookModel;
    }
    if (rolesList.contains(3)) {
        delete memberMapper;
        delete memberModel;
    }
    rolesList.clear();
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
    ui->hopThuButton->show();
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
        requestBookModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
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
        ui->muonSachTable->setColumnHidden(requestBookModel->fieldIndex("account_book_id"), true);
        ui->muonSachTable->resizeColumnsToContents();
        ui->dangMuonTable->setModel(requestBookModel);
        ui->dangMuonTable->setColumnHidden(requestBookModel->fieldIndex("account_book_id"), true);
        ui->dangMuonTable->resizeColumnsToContents();
        ui->matSachTable->setModel(requestBookModel);
        ui->matSachTable->setColumnHidden(requestBookModel->fieldIndex("account_book_id"), true);
        ui->matSachTable->resizeColumnsToContents();
        on_tabWidget_currentChanged(ui->tabWidget->currentIndex());
    }
    if (rolesList.contains(3)) {
        // Thiết lập memberModel
        memberModel = new QSqlRelationalTableModel(new QObject(), db);
        memberModel->setTable("account");
        int genderIdx = memberModel->fieldIndex("gender_id");
        int statusIdx = memberModel->fieldIndex("status_id");
        memberModel->setRelation(genderIdx, QSqlRelation("gender", "gender_id", "gender"));
        memberModel->setRelation(statusIdx, QSqlRelation("status", "status_id", "status"));

        QSqlTableModel *relModelGender = memberModel->relationModel(genderIdx);
        QSqlTableModel *relModelStatus = memberModel->relationModel(statusIdx);
        ui->gioiTinh->setModel(relModelGender);
        ui->gioiTinh->setModelColumn(relModelGender->fieldIndex("gender"));
        ui->tinhTrang->setModel(relModelStatus);
        ui->tinhTrang->setModelColumn(relModelStatus->fieldIndex("status"));

        memberModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
        memberModel->setHeaderData(1, Qt::Horizontal, tr("Tên đăng nhập"));
        memberModel->setHeaderData(2, Qt::Horizontal, tr("Mật khẩu"));
        memberModel->setHeaderData(3, Qt::Horizontal, tr("Tình trạng"));
        memberModel->setHeaderData(4, Qt::Horizontal, tr("Họ và tên"));
        memberModel->setHeaderData(5, Qt::Horizontal, tr("CMND"));
        memberModel->setHeaderData(6, Qt::Horizontal, tr("Giới tính"));
        memberModel->setHeaderData(7, Qt::Horizontal, tr("Ngày sinh"));
        memberModel->setHeaderData(8, Qt::Horizontal, tr("Email"));
        memberModel->setHeaderData(9, Qt::Horizontal, tr("Nghề nghiệp"));
        memberModel->setHeaderData(10, Qt::Horizontal, tr("Ảnh đại diện"));
        memberModel->select();
        // Thiết lập View
        ui->danhSachThanhVien->setModel(memberModel);
        ui->danhSachThanhVien->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->danhSachThanhVien->setItemDelegate(new AccountDelegate(this));
        ui->danhSachThanhVien->horizontalHeader()->setVisible(true);
        ui->danhSachThanhVien->setColumnHidden(2, true);
        ui->danhSachThanhVien->resizeColumnsToContents();
        memberMapper = new QDataWidgetMapper(this);
        memberMapper->setModel(memberModel);
        memberMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
        memberMapper->setItemDelegate(new AccountDelegate(this));;
        memberMapper->addMapping(ui->id, memberModel->fieldIndex("account_id"));
        memberMapper->addMapping(ui->tenDangNhap, memberModel->fieldIndex("account"));
        memberMapper->addMapping(ui->matKhau, memberModel->fieldIndex("password"));
        memberMapper->addMapping(ui->tinhTrang, memberModel->fieldIndex("status_id"));
        memberMapper->addMapping(ui->hoVaTen, memberModel->fieldIndex("fullname"));
        memberMapper->addMapping(ui->CMND, memberModel->fieldIndex("identity_number"));
        memberMapper->addMapping(ui->gioiTinh, genderIdx);
        memberMapper->addMapping(ui->tinhTrang, statusIdx);
        memberMapper->addMapping(ui->email, memberModel->fieldIndex("email"));
        memberMapper->addMapping(ui->congViec, memberModel->fieldIndex("job"));
        memberMapper->addMapping(ui->ngaySinh, memberModel->fieldIndex("birthdate"));
        memberMapper->addMapping(ui->avatar, memberModel->fieldIndex("avatar"));
        memberMapper->toFirst();

        checkVt();

        QObject::connect(ui->danhSachThanhVien->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), memberMapper, SLOT(setCurrentModelIndex(QModelIndex)));
        QObject::connect(ui->danhSachThanhVien->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(checkVt()));
    }
}

void MainWindow::on_muonButton_clicked()
{
    if (user_id != 0) {
        // Lấy danh sách đã chọn
        QModelIndexList list = ui->danhMucSach->selectionModel()->selectedRows(0);
        // Kiểm tra số lượng sách chờ duyệt hoặc đang mượn
        int num_borrowed = information->getBorrowedNumOfBook();
        if (num_borrowed + list.size() > 2) {
            QMessageBox::warning(this, "Mượn sách", QString("Bạn chỉ được mượn tối đa 2 cuốn một lúc!\nSố lượng đang mượn/chờ duyệt: %1").arg(num_borrowed));
            return;
        }
        // list không thể trống vì nếu empty thì muonButton sẽ bị vô hiệu hóa, do vậy không cần
        // kiểm tra xem list có trống hay không
        for (QModelIndex &index: list) {
            if (!index.sibling(index.row(), model->fieldIndex("status")).data().toInt()) {
                QMessageBox::warning(this, "Mượn sách", QString("Một số sách đã chọn không khả dụng!"));
                return;
            }
        }
        bool ok = true;
        // Tạo transaction và commit nó nếu không có lỗi
        if (db.transaction()) {
            QSqlQuery query(0, db);
            query.prepare("INSERT INTO account_book(account_id, book_id, book_status_id) VALUES(?, ?, ?)");
            for (QModelIndex index: list) {
                query.addBindValue(user_id);
                query.addBindValue(index.data().toString());
                query.addBindValue(1);
                if (!(ok &= query.exec())) {
                    qDebug() << query.lastError();
                    break;
                }
            }
            ok &= ok ? db.commit() : db.rollback();
        } else {
            ok = false;
        }
        if (ok) {
            QMessageBox::information(this, "Mượn sách", "Bạn đã gửi yêu cầu mượn sách!");
        } else {
            qDebug() << db.lastError();
            QMessageBox::warning(this, "Mượn sách", "Đã có lỗi xảy ra!");
        }
    } else {
        on_dangNhapButton_clicked();
    }
}

void MainWindow::on_chapThuanButton_clicked()
{
    QModelIndexList list = ui->muonSachTable->selectionModel()->selectedRows(requestBookModel->fieldIndex("book_status"));
    if (!list.isEmpty()) {
        bool ok = true;
        // Lấy danh sách tất cả sách đã chọn
        for (QModelIndex &i: list) {
            // Set trạng thái thành "Đang mượn"
            if (!(ok = requestBookModel->setData(i, 2))) {
                break;
            }
            // Nhập ngày mượn và ngày cần trả
            QDate today = QDate::currentDate();
            if (!(ok = requestBookModel->setData(i.sibling(i.row(), requestBookModel->fieldIndex("start_date")), today))) {
                break;
            }
            if (!(ok = requestBookModel->setData(i.sibling(i.row(), requestBookModel->fieldIndex("due_date")), today.addDays(15)))) {
                break;
            }
            // Gửi tin nhắn đến thành viên
            QString username = i.sibling(i.row(), accountIdIdx).data().toString();
            QString bookName = i.sibling(i.row(), bookIdIdx).data().toString();
            int request_id = i.sibling(i.row(), 0).data().toInt();
            QString text = QString("Bạn đã mượn thành công cuốn %1. Trong vòng 15 ngày bạn vui lòng đến cơ sở gần nhất để trả sách. Chúc bạn đọc sách vui vẻ!").arg(bookName);
            QString title = QString("LIBPRO - Mượn Sách #%1: Mượn sách thành công!").arg(request_id);
            ok = inbox->sendMessage(username, title, text);
        }
        if (ok) {
            // Gửi yêu cầu
            requestBookModel->submitAll();
            // Cập nhật lại model
            requestBookModel->select();
            QMessageBox::information(this, "Thành công!", "Bạn đã cho phép độc giả mượn sách!");
        } else {
            // Reset model
            requestBookModel->select();
            QMessageBox::warning(this, "Thất bại!", "Đã có lỗi xảy ra!");
        }
    }
}

void MainWindow::on_tuChoiButton_clicked()
{
    QModelIndexList list = ui->muonSachTable->selectionModel()->selectedRows(requestBookModel->fieldIndex("book_status"));
    if (!list.isEmpty()) {
        bool ok = true;
        // Lấy danh sách tất cả sách đã chọn
        for (QModelIndex &i: list) {
            // Lấy id của yêu cầu trước khi xóa
            int request_id = i.sibling(i.row(), 0).data().toInt();
            QString username = i.sibling(i.row(), accountIdIdx).data().toString();
            QString bookName = i.sibling(i.row(), bookIdIdx).data().toString();
            // Xóa yêu cầu
            if (!(ok = requestBookModel->removeRow(i.row()))) {
                break;
            }
            // Gửi tin nhắn đến thành viên
            QString text = QString("Chào bạn! Hiện tại yêu cầu mượn cuốn %1 không thể thực hiện được. Xin bạn vui lòng thử lại vào thời gian sau.").arg(bookName);
            QString title = QString("LIBPRO - Mượn sách #%1: Mượn sách không thành công!").arg(request_id);
            ok = inbox->sendMessage(username, title, text);
        }
        if (ok) {
            requestBookModel->submitAll();
            requestBookModel->select();
            QMessageBox::information(this, "Thành công!", "Bạn đã từ chối yêu cầu của độc giả!");
        } else {
            requestBookModel->select();
            QMessageBox::warning(this, "Thất bại!", "Đã có lỗi xảy ra!");
        }
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
        FineDialog *fineDialog = new FineDialog(this, db, list, inbox);
        fineDialog->setWindowTitle("Phạt");
        fineDialog->raise();
        fineDialog->setModal(true);
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
        for (QModelIndex &i: list) {
            // Lấy id của yêu cầu trước khi xóa
            int request_id = i.sibling(i.row(), 0).data().toInt();
            QString username = i.sibling(i.row(), accountIdIdx).data().toString();
            QString bookName = i.sibling(i.row(), bookIdIdx).data().toString();
            // Gửi tin nhắn đến thành viên
            QString text = QString("Thư viện xác nhận bạn đã trả sách %1!").arg(bookName);
            QString title = QString("LIBPRO - Trả sách #%1: Đã xác nhận trả sách!").arg(request_id);
            inbox->sendMessage(username, title, text);
        }
        requestBookModel->submitAll();
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
    ui->tuaDe->setReadOnly(false);
    ui->noiDungSach->setReadOnly(false);
    ui->tinhTrangSach->setEnabled(true);
    ui->thayDoiSachButton->hide();
    ui->chapNhanSachButton->show();
    ui->huySachButton->show();
}

void MainWindow::on_chapNhanSachButton_clicked()
{
    // Submit dữ liệu vào model
    bookMapper->submit();
    bookMapper->toNext();
    // Model update view
    model->select();
    ui->maSach->setReadOnly(true);
    ui->namSanXuat->setReadOnly(true);
    ui->tacGia->setReadOnly(true);
    ui->tuaDe->setReadOnly(true);
    ui->noiDungSach->setReadOnly(true);
    ui->tinhTrangSach->setEnabled(false);
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
    ui->tuaDe->setReadOnly(true);
    ui->noiDungSach->setReadOnly(true);
    ui->thayDoiSachButton->show();
    ui->chapNhanSachButton->hide();
    ui->huySachButton->hide();
}

void MainWindow::on_themSachButton_clicked()
{
    AddBook *addBook = new AddBook(0, db);
    addBook->setModal(true);
    addBook->setWindowTitle("Thêm sách");
    // Tự động giải phóng bộ nhớ sau khi tắt
    addBook->setAttribute(Qt::WA_DeleteOnClose);
    addBook->show();
    addBook->raise();
    // Cập nhật lại model nếu có sách mới thêm vào
    QObject::connect(addBook, SIGNAL(accepted()), model, SLOT(select()));
}

void MainWindow::on_hopThuButton_clicked()
{
    // Reset lại text trên hộp thư nếu có tin nhắn mới
    if (ui->hopThuButton->text() != "Hộp thư") {
        ui->hopThuButton->setText("Hộp thư");
    }
    emit inboxRequest();
}

void MainWindow::enableButtonsProperly()
{
    QModelIndexList list = ui->danhMucSach->selectionModel()->selectedRows();
    if (!list.isEmpty()) {
        ui->hienThiSach->show();
        ui->thayDoiSachButton->setEnabled(true);
        ui->muonButton->setEnabled(true);
    } else {
        ui->hienThiSach->hide();
        ui->thayDoiSachButton->setEnabled(false);
        ui->muonButton->setEnabled(false);
    }
}

void MainWindow::on_timKiemButton_2_clicked()
{
    on_thanhTimKiem_2_returnPressed();
}

void MainWindow::on_tinNhanMoi(int numMessages)
{
    if (numMessages < 1) {
        ui->hopThuButton->setText("Hộp thư");
    } else {
        ui->hopThuButton->setText(QString("Hộp thư (%1)").arg(numMessages));
    }
}

void MainWindow::on_thanhTimKiem_2_returnPressed()
{


//    QString type = ui->luaChon->text();
    QString keyword = ui->thanhTimKiem_2->text();

    QString andString;
    if (ui->combo_gioitinh->currentText()!="Tất cả")
    {
        andString += QString("gender LIKE '%1' AND ").arg(ui->combo_gioitinh->currentText());
    }
    if (ui->combo_tinhtrang->currentText()!="Tất cả")
    {
        andString += QString("status LIKE '%1' AND ").arg(ui->combo_tinhtrang->currentText());
    }
    QString orString;
    if (ui->f_id->isChecked())
    {
        orString += "account_id LIKE '%%1%' OR ";
    }

    if (ui->f_tendangnhap->isChecked())
    {
        orString += "account LIKE '%%1%%' OR ";
    }

    if (ui->f_hovaten->isChecked())
    {
        orString += "fullname LIKE '%%1%' OR ";
    }

    if (ui->f_cmnd->isChecked())
    {
        orString += "identity_number LIKE '%%1%' OR ";
    }

    // Magic
    andString = andString.isEmpty() ? "1" : andString + "1";
    orString = orString.isEmpty() ? "1" : orString + "0";
    QString s = QString("%1 AND (%2)").arg(andString, orString);
    memberModel->setFilter(keyword.isEmpty() ? "" : s.arg(keyword));
    memberModel->select();
}

void MainWindow::on_xoaButton_clicked()
{
    QSqlQuery query(0, db);
    query.prepare("delete from account_role where account_id= :id");
    query.bindValue(":id", ui->id->text());
    query.exec();

    query.prepare("delete from account where account_id= :id");
    query.bindValue(":id", ui->id->text());
    bool ms=query.exec();
    if (ms) {
        QMessageBox::about(this,"Xóa tài khoản","Xóa tài khoản thành công!");
        memberModel->select();
    }
    else
        QMessageBox::about(this,"Xóa tài khoản","Xóa tài khoản thất bại!");
}

void MainWindow::submitVt()
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
    bool ms = true;
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

void MainWindow::on_thayDoiButton_clicked()
{

    QString errorMessage;
    if (!ui->matKhau->hasAcceptableInput()) {
        errorMessage += "Mật khẩu phải có ít nhất 10 ký tự";
    }
    if (!ui->congViec->hasAcceptableInput()) {
        errorMessage += "Họ và tên không hợp lệ!\n";
    }
    if (!ui->congViec->hasAcceptableInput()) {
        errorMessage += "Công việc không hợp lệ!\n";
    }
    if (!ui->email->hasAcceptableInput()) {
        errorMessage += "Email không hợp lệ!\n";
    }
    if (!ui->CMND->hasAcceptableInput()) {
        errorMessage += "CMND không hợp lệ";
    }
    if (!errorMessage.isEmpty()) {
        QMessageBox::warning(this, "Không hợp lệ", errorMessage);
        return;
    }

    memberMapper->submit();
    submitVt();

    QSqlQuery query(0,db);
}

void MainWindow::checkVt() {

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

void MainWindow::on_managerAvatarButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), QString(), tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty()) {
         QPixmap pixmap(fileName);
         pixmap = pixmap.scaled(180, 180);
         ui->avatar->setPixmap(pixmap);
    }
}
