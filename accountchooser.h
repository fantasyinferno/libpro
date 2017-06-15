#ifndef ACCOUNTCHOOSER_H
#define ACCOUNTCHOOSER_H

#include <QDialog>
#include <QList>
#include <QSqlDatabase>
namespace Ui {
class AccountChooser;
}

class AccountChooser : public QDialog
{
    Q_OBJECT

public:
    explicit AccountChooser(QWidget *parent = 0, QSqlDatabase db = QSqlDatabase());
    ~AccountChooser();
private:
    Ui::AccountChooser *ui;
    QList<int> roleList;
    QSqlDatabase db;
private slots:
    on_dangNhapThanhCong(int, QString);
};

#endif // ACCOUNTCHOOSER_H
