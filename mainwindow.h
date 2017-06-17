#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QItemSelection>
#include <QSqlRelationalTableModel>
#include <QLabel>
#include "information.h"
#include "introform.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, QSqlDatabase database = QSqlDatabase());
    void initializeGUILogic(QSqlDatabase);
    void initializeDatabase(QSqlDatabase);
    void initializeTable();
    void initializeQuotes();
    QSqlDatabase getDatabase();
    QString getUser();
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    // Model thể hiện thông tin lấy từ cơ sở dữ liệu
    QSqlTableModel *model;
    QSqlRelationalTableModel *requestBookModel;
    int bookIdIdx, accountIdIdx, bookStatusIdIdx;
    // Cơ sở dữ liệu.
    QSqlDatabase db;
    // Tên truy cập và id
    QString user;
    int user_id;
    // Vai trò
    QList<int> rolesList;
signals:
    void updateMyBooks(const QModelIndexList&);
    void formRequest(int);
    void dangXuat();
    void informationRequest();
    void aboutTriggered();
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
};


#endif // MAINWINDOW_H
