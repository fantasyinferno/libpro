#ifndef LIBRARIANGUI_H
#define LIBRARIANGUI_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QItemSelection>
#include <QLabel>
#include "information.h"
#include "introform.h"

namespace Ui {
class LibrarianGUI;
}

class LibrarianGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit LibrarianGUI(QWidget *parent = 0, QSqlDatabase database = QSqlDatabase());
    void initializeGUILogic(QSqlDatabase);
    void initializeDatabase(QSqlDatabase);
    void initializeTable();
    void initializeQuotes();
    QSqlDatabase getDatabase();
    QString getUser();
    ~LibrarianGUI();

private:
    Ui::LibrarianGUI *ui;
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
private slots:
    void on_thanhTimKiem_returnPressed();
    void on_dangXuatButton_clicked();
    void on_dangNhapButton_clicked();
    void on_dangKyButton_clicked();
    void on_dangNhapThanhCong(int, QString);
    void on_username_clicked();
    void on_avatarChanged(const QPixmap*);
    void on_chuyenVaiTroButton_clicked();
    void on_actionAbout_LIBPRO_triggered();
};

#endif // LIBRARIANGUI_H
