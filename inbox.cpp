#include "inbox.h"
#include "ui_inbox.h"
#include "mainwindow.h"
#include "messagemodel.h"
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
    this->setModal(true);
    db = database;
    model = new QSqlRelationalTableModel(this, db);
    model->setTable("message");
    model->setHeaderData(model->fieldIndex("receiver"), Qt::Horizontal, tr("Người nhận"));
    model->setHeaderData(model->fieldIndex("sender"), Qt::Horizontal, tr("Người gửi"));
    model->setHeaderData(model->fieldIndex("title"), Qt::Horizontal, tr("Tiêu đề"));
    model->setHeaderData(model->fieldIndex("content"), Qt::Horizontal, tr("Nội dung"));
    model->setHeaderData(model->fieldIndex("send_at"), Qt::Horizontal, tr("Thời gian"));
    messageModel = new MessageModel(this);
    messageModel->setSourceModel(model);

    // Hộp thư đến và đi
    ui->hopThuDenTable->setModel(messageModel);
    ui->hopThuDenTable->setItemDelegate(new QSqlRelationalDelegate(this));
    ui->hopThuDenTable->setColumnHidden(model->fieldIndex("message_id"), true);
    ui->hopThuDenTable->setColumnHidden(model->fieldIndex("receiver"), true);
    ui->hopThuDenTable->setColumnHidden(model->fieldIndex("is_read"), true);
    ui->hopThuDenTable->sortByColumn(model->fieldIndex("send_at"), Qt::DescendingOrder);
    ui->hopThuDenTable->resizeColumnToContents(0);
    ui->hopThuDenTable->resizeColumnToContents(1);
    ui->hopThuDenTable->resizeColumnToContents(3);
    connect(ui->hopThuDenTable->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(handleHienThiTinNhan()));
    ui->hopThuDiTable->setModel(model);
    ui->hopThuDiTable->setItemDelegate(new QSqlRelationalDelegate(this));
    ui->hopThuDiTable->setColumnHidden(model->fieldIndex("message_id"), true);
    ui->hopThuDiTable->setColumnHidden(model->fieldIndex("sender"), true);
    ui->hopThuDiTable->setColumnHidden(model->fieldIndex("is_read"), true);
    ui->hopThuDiTable->setColumnHidden(0, true);
    ui->hopThuDiTable->sortByColumn(model->fieldIndex("send_at"), Qt::DescendingOrder);
    ui->hopThuDiTable->resizeColumnToContents(0);
    ui->hopThuDiTable->resizeColumnToContents(1);
    ui->hopThuDiTable->resizeColumnToContents(3);
    connect(ui->hopThuDenTable->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(handleHienThiTinNhan()));

    int receiverIdx = model->fieldIndex("receiver");
    int senderIdx = model->fieldIndex("sender");
    model->setRelation(receiverIdx, QSqlRelation("account", "account_id", "account"));
    model->setRelation(senderIdx, QSqlRelation("account", "account_id", "account"));
    model->select();
    int contentIdx = model->fieldIndex("content");
    int titleIdx = model->fieldIndex("title");
    int send_atIdx = model->fieldIndex("send_at");
    QDataWidgetMapper *hopThuDenMapper = new QDataWidgetMapper(this);
    hopThuDenMapper->setItemDelegate(new QSqlRelationalDelegate(this));
    hopThuDenMapper->setModel(messageModel);
    hopThuDenMapper->addMapping(ui->loiNhanTextEdit, contentIdx);
    hopThuDenMapper->addMapping(ui->tieuDeLineEdit, titleIdx);
    hopThuDenMapper->addMapping(ui->nguoiGuiLineEdit, senderIdx);
    hopThuDenMapper->addMapping(ui->thoiGianLineEdit, send_atIdx);
    connect(ui->hopThuDenTable->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), hopThuDenMapper, SLOT(setCurrentModelIndex(QModelIndex)));
    QDataWidgetMapper *hopThuDiMapper = new QDataWidgetMapper(this);
    hopThuDiMapper->setItemDelegate(new QSqlRelationalDelegate(this));
    hopThuDiMapper->setModel(model);
    hopThuDiMapper->addMapping(ui->loiNhanTextEdit, contentIdx);
    hopThuDiMapper->addMapping(ui->tieuDeLineEdit, titleIdx);
    hopThuDiMapper->addMapping(ui->nguoiGuiLineEdit, receiverIdx);
    hopThuDiMapper->addMapping(ui->thoiGianLineEdit, send_atIdx);
    connect(ui->hopThuDiTable->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), hopThuDiMapper, SLOT(setCurrentModelIndex(QModelIndex)));
    connect(ui->hopThuDenTable->selectionModel(), SIGNAL(currentRowChanged(QModelIndex, QModelIndex)), this, SLOT(on_messageRead(QModelIndex)));
    ui->nguoiNhan->setModel(model->relationModel(receiverIdx));
    ui->nguoiNhan->setModelColumn(1);
    ui->hienThiTinNhan->hide();
}

bool Inbox::sendMessage(QString username, QString title, QString text)
{
    QSqlQuery query(0, db);
    query.prepare("SELECT account_id FROM account WHERE account = ?");
    query.addBindValue(username);
    if (!query.exec()) {
        qDebug() << query.lastError();
        return false;
    }
    if (!query.next()) {
        qDebug() << query.lastError();
        return false;
    }
    int receiver_id = query.value(0).toInt();
    // Gửi tin nhắn
    query.prepare("INSERT INTO message(sender, receiver, title, content) VALUES(?, ?, ?, ?)");
    query.addBindValue(user_id);
    query.addBindValue(receiver_id);
    query.addBindValue(title);
    query.addBindValue(text);
    if (!query.exec()) {
        qDebug() << query.lastError();
        return false;
    }
    return true;
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
    model->setFilter(QString("is_read = 0 AND receiver = %1").arg(user_id));
    emit tinNhanMoi(model->rowCount());
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
        if (sendMessage(ui->nguoiNhan->currentText(), ui->tuaDe->text().isEmpty() ? tr("Không có tiêu đề") : ui->tuaDe->text(), ui->loiNhan->toPlainText())) {
            model->select();
            ui->loiNhan->clear();
            ui->nguoiNhan->clearEditText();
            ui->tuaDe->clear();
            QMessageBox::information(this, tr("Gửi tin nhắn"), tr("Đã gửi tin nhắn thành công!"));
        } else {
            QMessageBox::warning(this, tr("Gửi tin nhắn"), tr("Đã có lỗi xảy ra!"));
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
            // submit thông qua messageModel
            messageModel->setData(selected.sibling(selected.row(), model->fieldIndex("is_read")), true);
            messageModel->submit();
        }
    }
}


void Inbox::on_capNhatButton_clicked()
{
    model->select();
}

void Inbox::handleHienThiTinNhan()
{
    if (!ui->hopThuDenTable->selectionModel()->selectedRows().isEmpty() || !ui->hopThuDiTable->selectionModel()->selectedRows().isEmpty()) {
        ui->hienThiTinNhan->show();
    } else {
        ui->hienThiTinNhan->hide();
    }
}
