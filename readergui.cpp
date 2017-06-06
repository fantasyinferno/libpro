#include "readergui.h"
#include "ui_readergui.h"
#include <QSqlTableModel>
#include "bookdelegate.h"
#include <QDataWidgetMapper>
#include <QSqlQuery>
#include <QSqlRecord>
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

void ReaderGUI::on_selectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
    if (!ui->danhMucSach->selectionModel()->selectedRows(1).empty()) {
        QModelIndex bookPathIndex = ui->danhMucSach->selectionModel()->selectedRows(1).back();
        QString imagePath = ui->danhMucSach->model()->data(bookPathIndex).toString();
        QPixmap qp = QPixmap(":" + imagePath);
        ui->biaSach->setPixmap(qp);
        ui->biaSach->setScaledContents(true);
        ui->biaSach->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        ui->hienThiSach->show();
        ui->danhMucSach->selectionModel()->selectedRows();
    }
}

void ReaderGUI::on_thanhTimKiem_returnPressed()
{
    QString keyword = ui->thanhTimKiem->text();
    QString queryString = QString("book MATCH 'title:%1 OR author:%1 OR description:%1 OR book_id:%1'").arg(keyword);
    model->setFilter(queryString);
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
    ui->danhMucSach->setColumnWidth(1, 60);
    ui->danhMucSach->setColumnWidth(2, 400);
    ui->danhMucSach->setColumnWidth(3, 400);
    ui->danhMucSach->setColumnHidden(6, true);
//    for (int i = 0; i != model->rowCount(); ++i) {
//        QPushButton *muonButton = new QPushButton(tr("Mượn"));
//        QObject::connect(muonButton, SIGNAL(clicked()), this, SLOT(on_muon()));
//        ui->danhMucSach->setIndexWidget(model->index(i, 8), muonButton);
//    }
    QObject::connect(ui->danhMucSach->selectionModel(), SIGNAL(selectionChanged( const QItemSelection &, const QItemSelection &)), SLOT(on_selectionChanged(const QItemSelection &, const QItemSelection &)));
    QDataWidgetMapper *mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->addMapping(ui->maSach, model->fieldIndex("book_id"));
    mapper->addMapping(ui->biaSach, model->fieldIndex("cover"));
    mapper->addMapping(ui->noiDungSach, model->fieldIndex("description"));
    mapper->addMapping(ui->tuaDe, model->fieldIndex("title"));
    mapper->addMapping(ui->tacGia, model->fieldIndex("author"));
    mapper->addMapping(ui->namSanXuat, model->fieldIndex("year"));
    connect(ui->danhMucSach->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), mapper, SLOT(setCurrentModelIndex(QModelIndex)));
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
}

void ReaderGUI::on_dangXuatButton_clicked()
{
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
