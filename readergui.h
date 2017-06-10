#ifndef READERGUI_H
#define READERGUI_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QItemSelection>
#include "information.h"
#include "introform.h"
namespace Ui {
class ReaderGUI;
}

class ReaderGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit ReaderGUI(QWidget *parent = 0);
    void initializeGUILogic();
    void initializeDatabase();
    void initializeTable();
    void initializeQuotes();
    QSqlDatabase getDatabase();
    QString getUser();
    ~ReaderGUI();

private:
    Ui::ReaderGUI *ui;
    // Model thể hiện thông tin lấy từ cơ sở dữ liệu
    QSqlTableModel *model;
    IntroForm *introForm;
    Information *information;
    // Cơ sở dữ liệu.
    QSqlDatabase db;
    // Tên truy cập và id
    QString user;
    int user_id;
signals:
    void updateMyBooks(const QModelIndexList&);
private slots:
    void on_selectionChanged(const QItemSelection &, const QItemSelection &);
    void on_thanhTimKiem_returnPressed();
    void on_muon();
    void on_dangXuatButton_clicked();
    void on_dangNhapButton_clicked();
    void on_dangKyButton_clicked();
    void on_dangNhapThanhCong(int, QString);
    void on_username_clicked();
    void on_muonButton_clicked();
};

#endif // READERGUI_H
