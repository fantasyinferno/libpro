#ifndef ADDBOOK_H
#define ADDBOOK_H

#include <QDialog>
#include <QSqlDatabase>
namespace Ui {
class AddBook;
}

class AddBook : public QDialog
{
    Q_OBJECT

public:
    explicit AddBook(QWidget *parent = 0, QSqlDatabase database = QSqlDatabase());
    ~AddBook();

private slots:
    void on_submitButton_clicked();

    void on_cancelButton_clicked();

    void on_changeCoverButton_clicked();

private:
    Ui::AddBook *ui;
    QSqlDatabase db;
};

#endif // ADDBOOK_H
