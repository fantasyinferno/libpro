#ifndef INFORMATION_H
#define INFORMATION_H

#include <QWidget>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QItemSelection>


namespace Ui {
class Information;
}

class Information : public QWidget
{
    Q_OBJECT

public:
    explicit Information(QWidget *parent = 0);
    ~Information();
    void load(QString, QString);



private slots:
    // Mật khẩu
    void on_td_mk_clicked();
    void on_ok_mk_clicked();
    void on_ca_mk_clicked();

    // Tình trạng
    void on_td_tt_clicked();
    void on_ok_tt_clicked();
    void on_ca_tt_clicked();

    // Họ và tên
    void on_td_hvt_clicked();
    void on_ok_hvt_clicked();
    void on_ca_hvt_clicked();

    // Chứng minh nhân dân
    void on_td_cmnd_clicked();
    void on_ok_cmnd_clicked();
    void on_ca_cmnd_clicked();

    // Giới tính
    void on_td_gt_clicked();
    void on_ok_gt_clicked();
    void on_ca_gt_clicked();

    // Ngày sinh
    void on_td_ns_clicked();
    void on_ok_ns_clicked();
    void on_ca_ns_clicked();

    // Nghề nghiệp
    void on_td_nn_clicked();
    void on_ok_nn_clicked();
    void on_ca_nn_clicked();

    // Email
    void on_td_em_clicked();
    void on_ok_em_clicked();
    void on_ca_em_clicked();

    // Vai trof
    void on_td_vt_clicked();
    void on_ok_vt_clicked();
    void on_ca_vt_clicked();

    void on_ca_clicked();

signals:
private:
    QSqlDatabase db;
    Ui::Information *ui;
};

#endif // INFORMATION_H
