#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QItemSelection>
#include <QSqlRelationalTableModel>
#include <QLabel>
#include <QItemSelection>
#include "information.h"
#include "introform.h"
#include "about.h"
#include "inbox.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, QSqlDatabase database = QSqlDatabase());
    QSqlDatabase getDatabase();
    QString getUser();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    // Model thể hiện thông tin lấy từ cơ sở dữ liệu
    QSqlTableModel *model;
    QSqlRelationalTableModel *requestBookModel, *memberModel;
    int bookIdIdx, accountIdIdx, bookStatusIdIdx;
    // Mapper
    QDataWidgetMapper *bookMapper, *memberMapper;
    // Cơ sở dữ liệu.
    QSqlDatabase db;
    // Tên truy cập và id
    QString user;
    int user_id;
    // Vai trò
    QList<int> rolesList;
    // UI
    IntroForm *introForm;
    Information *information;
    About *about;
    Inbox *inbox;


signals:
    void updateMyBooks(const QModelIndexList&);
    void formRequest(int);
    void dangXuat();
    void informationRequest();
    void aboutTriggered();
    void inboxRequest();
private slots:
    void on_thanhTimKiem_returnPressed();
    void on_dangXuatButton_clicked();
    void on_dangNhapButton_clicked();
    void on_dangKyButton_clicked();
    void on_dangNhapThanhCong(int, QString);
    void on_username_clicked();
    void on_avatarChanged(const QPixmap*);
    void on_actionAbout_LIBPRO_triggered();
    void on_rolesLoaded(QList<int>&);
    void on_muonButton_clicked();
    void on_chapThuanButton_clicked();
    void on_tuChoiButton_clicked();
    void on_phatButton_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_hanNop_currentIndexChanged(int index);
    void on_xacNhanSachDaTraButton_clicked();
    void on_thayDoiSachButton_clicked();
    void on_chapNhanSachButton_clicked();
    void on_huySachButton_clicked();
    void initializeGUILogic(QSqlDatabase);
    void initializeDatabase(QSqlDatabase);
    void initializeTable();
    void initializeQuotes();
    void initializeButtons();
    void enableLibrarianButtons(bool, bool, bool, bool);
    void on_themSachButton_clicked();
    void on_hopThuButton_clicked();
    void enableButtonsProperly();
    // Kiểm tra vai trò (manager)
    void submitVt();
    void checkVt();
    void on_thayDoiButton_clicked();
    void on_xoaButton_clicked();
    void on_thanhTimKiem_2_returnPressed();
    void on_timKiemButton_2_clicked();
    // Kiểm tra tin nhắn mới
    void on_tinNhanMoi(int);
};


#endif // MAINWINDOW_H
