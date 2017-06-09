#ifndef INFORMATION_H
#define INFORMATION_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QItemSelection>
#include <QDataWidgetMapper>
#include <QSqlRelationalTableModel>


namespace Ui {
class Information;
}

class Information : public QDialog
{
    Q_OBJECT

public:
    explicit Information(QWidget *parent = 0);
    ~Information();
    void load(QString, QString);
    void submitVt();
    void checkVt();
    void enableEdit(bool);


private slots:
    void on_hoanTatButton_clicked();
    void on_thayDoiButton_clicked();
    void on_dangNhapThanhCong(QString);

    void on_huyButton_clicked();

signals:
private:
    QSqlDatabase db;
    Ui::Information *ui;
    QString tdn;
    QSqlRelationalTableModel *model;
    QDataWidgetMapper *mapper;
};

#endif // INFORMATION_H
