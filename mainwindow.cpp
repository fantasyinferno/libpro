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

MainWindow::MainWindow(QWidget *parent, QSqlDatabase database) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initializeGUILogic(database);
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
    QDataWidgetMapper *mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->setItemDelegate(new BookDelegate(this));
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->addMapping(ui->maSach, model->fieldIndex("book_id"));
    mapper->addMapping(ui->biaSach, model->fieldIndex("cover"));
    mapper->addMapping(ui->noiDungSach, model->fieldIndex("description"));
    mapper->addMapping(ui->tuaDe, model->fieldIndex("title"));
    mapper->addMapping(ui->tacGia, model->fieldIndex("author"));
    mapper->addMapping(ui->namSanXuat, model->fieldIndex("year"));
    mapper->addMapping(ui->biaSach, model->fieldIndex("cover"));
    connect(ui->danhMucSach->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), mapper, SLOT(setCurrentModelIndex(QModelIndex)));
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
void MainWindow::initializeGUILogic(QSqlDatabase database) {
    initializeDatabase(database);
    initializeTable();
    initializeQuotes();
    connect(ui->timKiemButton, SIGNAL(clicked()), this, SLOT(on_thanhTimKiem_returnPressed()));
    ui->dangXuatButton->hide();
    ui->thayDoiSachButton->hide();
    ui->themSachButton->hide();
    ui->username->setEnabled(false);
    ui->toolBox->setItemEnabled(0, true);
    ui->toolBox->setItemEnabled(1, false);
    ui->toolBox->setItemEnabled(2, false);

}

void MainWindow::on_dangXuatButton_clicked()
{
    user = "";
    ui->username->setText("");
    ui->username->setEnabled(false);
    ui->dangXuatButton->hide();
    ui->thayDoiSachButton->hide();
    ui->themSachButton->hide();
    ui->dangKyButton->show();
    ui->dangNhapButton->show();
    ui->avatarIcon->clear();
    ui->toolBox->setItemEnabled(0, true);
    ui->toolBox->setItemEnabled(1, false);
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
        ui->danhMucSach->setEditTriggers(QAbstractItemView::DoubleClicked);
        ui->danhMucSach->setSelectionBehavior(QAbstractItemView::SelectItems);
        ui->thayDoiSachButton->show();
        ui->themSachButton->show();
        requestBookModel = new QSqlTableModel(this, db);
        requestBookModel->setTable("account_book");
        requestBookModel->setHeaderData(requestBookModel->fieldIndex("book_id"), Qt::Horizontal, tr("ISBN"));
        requestBookModel->setHeaderData(requestBookModel->fieldIndex("account_id"), Qt::Horizontal, tr("Người mượn"));
        requestBookModel->setHeaderData(requestBookModel->fieldIndex("start_date"), Qt::Horizontal, tr("Ngày ký mượn"));
        requestBookModel->setHeaderData(requestBookModel->fieldIndex("due_date"), Qt::Horizontal, tr("Ngày cần trả"));

        ui->yeuCauMuonSach->setModel(requestBookModel);
        ui->yeuCauMuonSach->setItemDelegate(new QSqlRelationalDelegate(this));
        ui->yeuCauMuonSach->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->yeuCauMuonSach->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->yeuCauMuonSach->setColumnHidden(requestBookModel->fieldIndex("start_date"), true);
        ui->yeuCauMuonSach->setColumnHidden(requestBookModel->fieldIndex("due_date"), true);
        ui->yeuCauMuonSach->setColumnHidden(requestBookModel->fieldIndex("book_status_id"), true);

        ui->sachDangMuon->setModel(requestBookModel);
        ui->sachDangMuon->setItemDelegate(new QSqlRelationalDelegate(this));
        ui->sachDangMuon->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->sachDangMuon->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->sachDangMuon->setColumnHidden(requestBookModel->fieldIndex("book_status_id"), true);

        ui->sachBiMat->setModel(requestBookModel);
        ui->sachBiMat->setItemDelegate(new QSqlRelationalDelegate(this));
        ui->sachBiMat->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->sachBiMat->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->sachBiMat->setColumnHidden(requestBookModel->fieldIndex("book_status_id"), true);

        ui->sachQuaHan->setModel(requestBookModel);
        ui->sachQuaHan->setItemDelegate(new QSqlRelationalDelegate(this));
        ui->sachQuaHan->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->sachQuaHan->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->sachQuaHan->setColumnHidden(requestBookModel->fieldIndex("book_status_id"), true);

    }
}

void MainWindow::on_muonButton_clicked()
{

    QModelIndexList list = ui->danhMucSach->selectionModel()->selectedRows(0);
    if (!list.isEmpty()) {
        QSqlQuery query(0, db);
        query.prepare("INSERT INTO account_book(account_id, book_id, book_status_id) VALUES(?, ?, ?)");
        for (QModelIndex index: list) {
            query.addBindValue(user_id);
            query.addBindValue(index.data().toString());
            query.addBindValue(1);
            if (!query.exec()) {
                qDebug() << query.lastError();
            }
        }
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index == 0) {
        // Yêu cầu mượn
        requestBookModel->setFilter("book_status_id = 1");
        requestBookModel->select();
    } else if (index == 1) {
        // Sách đang mượn
        requestBookModel->setFilter("book_status_id = 2");
        requestBookModel->select();


    } else if (index == 2) {
        // Sách quá hạn
        requestBookModel->setFilter("julianday(due_date) - julianday(current_date)");
        requestBookModel->select();

    } else {
        // Sách bị mất
        requestBookModel->setFilter("book_status_id = 4");
        requestBookModel->select();

    }
}

void MainWindow::on_chapThuanButton_clicked()
{
    QModelIndexList list = ui->yeuCauMuonSach->selectionModel()->selectedRows(requestBookModel->fieldIndex("book_status_id"));
    if (!list.isEmpty()) {
        for (QModelIndex i: list) {
            requestBookModel->setData(i, 2);
        }
        QMessageBox::information(this, "Thành công!", "Bạn đã cho phép độc giả mượn sách!");
        // Gửi tin nhắn đến thành viên
    }
}

void MainWindow::on_tuChoiButton_clicked()
{
    QModelIndexList list = ui->yeuCauMuonSach->selectionModel()->selectedRows(requestBookModel->fieldIndex("book_status_id"));
    if (!list.isEmpty()) {
        for (QModelIndex i: list) {
            requestBookModel->removeRow(i.row());
        }
        QMessageBox::information(this, "Thành công!", "Bạn đã từ chối yêu cầu của độc giả!");
        // Gửi tin nhắn đến thành viên
    }
}
