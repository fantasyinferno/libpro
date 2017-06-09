#ifndef INTROFORM_H
#define INTROFORM_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QItemSelection>


namespace Ui {
class IntroForm;
}

class IntroForm : public QDialog
{
    Q_OBJECT

public:
    explicit IntroForm(QWidget *parent = 0);
    ~IntroForm();

    void setTab(int);

private slots:
    void on_pushButton_clicked();

    void on_dangKyButton_clicked();

signals:
    void dangNhapThanhCong(QString);



private:
    QSqlDatabase db;
    Ui::IntroForm *ui;
};

#endif // INTROFORM_H
