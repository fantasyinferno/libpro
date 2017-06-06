#ifndef INTROFORM_H
#define INTROFORM_H

#include <QWidget>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QItemSelection>


namespace Ui {
class IntroForm;
}

class IntroForm : public QWidget
{
    Q_OBJECT

public:
    explicit IntroForm(QWidget *parent = 0);
    ~IntroForm();

    void set_tab(int);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

signals:
    void dangNhapThanhCong(QString);



private:
    QSqlDatabase db;
    Ui::IntroForm *ui;
};

#endif // INTROFORM_H
