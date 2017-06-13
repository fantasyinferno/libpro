#include "readergui.h"
#include "ui_readergui.h"
#include <QSqlTableModel>
#include "bookdelegate.h"
#include <QDataWidgetMapper>
#include <QSqlQuery>
#include <qDebug>
#include <QDate>
#include <QSqlRecord>
#include <QSqlError>
#include <QByteArray>

ReaderGUI::ReaderGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReaderGUI)
{
    ui->setupUi(this);
    initializeGUILogic();
}

ReaderGUI::~ReaderGUI()
{
    delete ui;
}
QSqlDatabase ReaderGUI::getDatabase() {
    return db;
}
QString ReaderGUI::getUser() {
    return user;
}

void ReaderGUI::on_thanhTimKiem_returnPressed()
{
    QString keyword = ui->thanhTimKiem->text();
    if (!keyword.isEmpty()) {
        QString queryString = QString("book MATCH 'title:%1 OR author:%1 OR description:%1 OR book_id:%1'").arg(keyword);
        model->setFilter(queryString);
    }
}


void ReaderGUI::on_muon() {

}
void ReaderGUI::initializeDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("F:/SoftwareDevelopment/Qt/libpro/libpro.db");
    if (!db.open()) {
        return;
    }
}

void ReaderGUI::initializeTable()
{
    // Thiết lập model
    model = new QSqlTableModel(new QObject(), db);
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
void ReaderGUI::initializeQuotes() {
    QSqlQuery quoteQuery(db);
    quoteQuery.exec("SELECT * FROM quotes WHERE rowid >= (abs(random()) % (SELECT MAX(rowid) FROM quotes))");
    quoteQuery.next();
    QString quote = quoteQuery.value(0).toString();
    QString author = quoteQuery.value(1).toString();
    ui->quotesLabel->setText("'" + quote + "'\n - " + author);
    this->show();
}
void ReaderGUI::initializeGUILogic() {
    initializeDatabase();
    initializeTable();
    initializeQuotes();
    introForm = new IntroForm(this);
    introForm->setWindowTitle("Đăng nhập/Đăng ký");
    information = new Information(this);
    information->setWindowTitle("Thông tin cá nhân");
    connect(introForm, SIGNAL(dangNhapThanhCong(int, QString)), this, SLOT(on_dangNhapThanhCong(int, QString)));
    connect(introForm, SIGNAL(dangNhapThanhCong(int, QString)), information, SLOT(on_dangNhapThanhCong(int, QString)));
    connect(information, SIGNAL(avatarChanged(const QPixmap*)), this, SLOT(on_avatarChanged(const QPixmap*)));
    connect(this, SIGNAL(updateMyBooks(const QModelIndexList&)), information, SLOT(on_updateMyBooks(const QModelIndexList&)));
    connect(ui->timKiemButton, SIGNAL(clicked()), this, SLOT(on_thanhTimKiem_returnPressed()));
    connect(this, SIGNAL(dangXuat()), information, SLOT(on_dangXuat()));
    ui->dangXuatButton->hide();
    ui->username->setEnabled(false);
}

void ReaderGUI::on_dangXuatButton_clicked()
{
    user = "";
    ui->username->setText("");
    ui->username->setEnabled(false);
    ui->dangXuatButton->hide();
    ui->dangKyButton->show();
    ui->dangNhapButton->show();
    ui->avatarIcon->clear();
    emit dangXuat();
}

void ReaderGUI::on_dangNhapButton_clicked()
{
    introForm->setTab(0);
    introForm->show();
}

void ReaderGUI::on_dangKyButton_clicked()
{
    introForm->setTab(1);
    introForm->show();
}

void ReaderGUI::on_dangNhapThanhCong(int id, QString username) {
    user = username;
    user_id = id;
    ui->dangKyButton->hide();
    ui->dangNhapButton->hide();
    ui->dangXuatButton->show();
    ui->username->setText(user);
    ui->username->setEnabled(true);
}

void ReaderGUI::on_username_clicked()
{
    information->show();
}

void ReaderGUI::on_muonButton_clicked()
{
    // Note: updateMyBooks should be connected to information's slot
    emit updateMyBooks(ui->danhMucSach->selectionModel()->selectedRows());
}

void ReaderGUI::on_avatarChanged(const QPixmap* pixmap) {
    ui->avatarIcon->setPixmap(*pixmap);
}
