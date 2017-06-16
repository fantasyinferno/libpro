#ifndef READERGUI_H
#define READERGUI_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QItemSelection>
#include <QLabel>
#include "information.h"
#include "introform.h"
namespace Ui {
class ReaderGUI;
}

class ReaderGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit ReaderGUI(QWidget *parent = 0, QSqlDatabase database = QSqlDatabase());
    void initializeGUILogic(QSqlDatabase);
    void initializeDatabase(QSqlDatabase);
    void initializeTable();
    void initializeQuotes();
    QSqlDatabase getDatabase();
    QString getUser();
    ~ReaderGUI();

private:
    Ui::ReaderGUI *ui;
    // Model thể hiện thông tin lấy từ cơ sở dữ liệu
    QSqlTableModel *model;
    // Cơ sở dữ liệu.
    QSqlDatabase db;
    // Tên truy cập và id
    QString user;
    int user_id;
signals:
    void updateMyBooks(const QModelIndexList&);
    void chuyenVaiTro();
    void formRequest(int);
    void dangXuat();
    void informationRequest();
    void aboutTriggered();
    void iAmYourParent(QWidget*);
private slots:
    void on_thanhTimKiem_returnPressed();
    void on_muon();
    void on_dangXuatButton_clicked();
    void on_dangNhapButton_clicked();
    void on_dangKyButton_clicked();
    void on_dangNhapThanhCong(int, QString);
    void on_username_clicked();
    void on_muonButton_clicked();
    void on_avatarChanged(const QPixmap*);
    void on_chuyenVaiTroButton_clicked();
    void on_actionAbout_LIBPRO_triggered();
    void on_roleChosen(int);
};

#endif // READERGUI_H
