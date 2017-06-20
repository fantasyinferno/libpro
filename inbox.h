#ifndef INBOX_H
#define INBOX_H
#include "messagemodel.h"
#include <QDialog>
#include <QSqlDatabase>
#include <QSqlRelationalTableModel>
#include <QDataWidgetMapper>
namespace Ui {
class Inbox;
}

class Inbox : public QDialog
{
    Q_OBJECT

public:
    explicit Inbox(QWidget *parent = 0, QSqlDatabase database = QSqlDatabase());
    // Hàm gửi tin nhắn đến thành viên
    bool sendMessage(QString, QString, QString);
    ~Inbox();

private slots:
    void on_toolBox_currentChanged(int index);
    void on_dangNhapThanhCong(int id, QString username);
    void on_dangXuat();
    void on_guiButton_clicked();
    void on_messageRead(QModelIndex);
    void on_capNhatButton_clicked();
signals:
    void tinNhanMoi(int);
private:
    Ui::Inbox *ui;
    QSqlDatabase db;
    MessageModel *messageModel;
    QSqlRelationalTableModel *model;

    int user_id;
    QString user;
};

#endif // INBOX_H
