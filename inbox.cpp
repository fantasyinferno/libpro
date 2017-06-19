#include "inbox.h"
#include "ui_inbox.h"
#include "mainwindow.h"
#include <QSqlQuery>
#include <QSqlRelationalDelegate>
#include <QSqlRelation>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
Inbox::Inbox(QWidget *parent, QSqlDatabase database) :
    QDialog(parent),
    ui(new Ui::Inbox)
{
    ui->setupUi(this);
    db = database;
    model = new QSqlRelationalTableModel(this, db);
    model->setTable("message");
    model->setHeaderData(model->fieldIndex("receiver"), Qt::Horizontal, tr("Người nhận"));
    model->setHeaderData(model->fieldIndex("sender"), Qt::Horizontal, tr("Thời điểm gửi"));
    model->setHeaderData(model->fieldIndex("title"), Qt::Horizontal, tr("Tiêu đề"));
    model->setHeaderData(model->fieldIndex("content"), Qt::Horizontal, tr("Nội dung"));
    model->setHeaderData(model->fieldIndex("send_at"), Qt::Horizontal, tr("Ngày gửi"));
    ui->hopThuDenTable->setModel(model);
    ui->hopThuDenTable->setItemDelegate(new QSqlRelationalDelegate(this));
    ui->hopThuDenTable->setColumnHidden(0, true);
    ui->hopThuDenTable->sortByColumn(model->fieldIndex("send_at"), Qt::AscendingOrder);
    ui->hopThuDiTable->setModel(model);
    ui->hopThuDiTable->setItemDelegate(new QSqlRelationalDelegate(this));
    ui->hopThuDiTable->setColumnHidden(0, true);
    ui->hopThuDenTable->sortByColumn(model->fieldIndex("send_at"), Qt::AscendingOrder);
    int receiverIdx = model->fieldIndex("receiver");
    int senderIdx = model->fieldIndex("sender");
    model->setRelation(receiverIdx, QSqlRelation("account", "account_id", "account"));
    model->setRelation(senderIdx, QSqlRelation("account", "account_id", "account"));
    model->select();
    QDataWidgetMapper *mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    mapper->addMapping(ui->loiNhanTextEdit, model->fieldIndex("content"));
    connect(ui->hopThuDenTable->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), mapper, SLOT(setCurrentModelIndex(QModelIndex)));
    connect(ui->hopThuDiTable->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), mapper, SLOT(setCurrentModelIndex(QModelIndex)));
    connect(ui->hopThuDenTable->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this, SLOT(on_messageRead(QModelIndex)));
    ui->nguoiNhan->setModel(model->relationModel(receiverIdx));
    ui->nguoiNhan->setModelColumn(1);
}

Inbox::~Inbox()
{
    delete ui;
}

void Inbox::on_toolBox_currentChanged(int index)
{
    if (index == 0) {
        model->setFilter(QString("receiver = %1").arg(user_id));
    } else {
        model->setFilter(QString("sender = %1").arg(user_id));
    }
}

void Inbox::on_dangNhapThanhCong(int id, QString username)
{
    user_id = id;
    user = username;
    on_toolBox_currentChanged(ui->toolBox->currentIndex());
}
void Inbox::on_dangXuat() {
    user_id = 0;
    user = "";
    model->setFilter("0");
}


void Inbox::on_guiButton_clicked()
{
    // Kiểm tra nội dung tin nhắn
    QString errorMessage;
    if (ui->loiNhan->toPlainText().isEmpty()) {
        errorMessage += "Nội dung tin nhắn không được bỏ trống!\n";
    }
    if (ui->nguoiNhan->findText(ui->nguoiNhan->currentText()) == -1) {
        errorMessage += "Hãy chọn một người nhận khả thi!";
    }
    if (errorMessage.isEmpty()) {
        bool ok = true;
        QSqlQuery query(0, db);
        query.prepare("SELECT account_id FROM account WHERE account = ?");
        query.addBindValue(ui->nguoiNhan->currentText());
        ok &= query.exec();
        query.next();
        int receiver_id = query.value(0).toInt();
        query.prepare("INSERT INTO message(sender, receiver, title, content) VALUES(?, ?, ?, ?)");
        query.addBindValue(user_id);
        query.addBindValue(receiver_id);
        query.addBindValue(ui->tuaDe->text().isEmpty() ? tr("(Không có tiêu đề)") :ui->tuaDe->text());
        query.addBindValue(ui->loiNhan->toPlainText());
        ok &= query.exec();
        if (ok) {
            model->select();
            QMessageBox::information(this, tr("Gửi tin nhắn thành công"), tr("Đã gửi tin nhắn thành công!"));
        } else {
            qDebug() << query.lastError();
            QMessageBox::warning(this, tr("Gửi tin nhắn thất bại"), tr("Đã có lỗi xảy ra!"));
        }
    } else {
        QMessageBox::warning(this, tr("Gửi tin nhắn"), errorMessage);
    }
}

void Inbox::on_messageRead(QModelIndex selected)
{
    if (selected.isValid()) {
        QModelIndex is_read = selected.sibling(selected.row(), model->fieldIndex("is_read"));
        if (is_read.data().toInt() == 0) {
            model->setData(selected.sibling(selected.row(), model->fieldIndex("is_read")), true);
            model->submit();
            model->select();
        }
    }
}
void Inbox::foo() {
    model->rowCount();
}
