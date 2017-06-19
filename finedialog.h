#ifndef FINEDIALOG_H
#define FINEDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QModelIndexList>
#include "inbox.h"
namespace Ui {
class FineDialog;
}

class FineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FineDialog(QWidget *parent = 0, QSqlDatabase database = QSqlDatabase(), QModelIndexList list = QModelIndexList(), Inbox *inbox = new Inbox());
    ~FineDialog();

private slots:
    void on_hinhThuc_currentIndexChanged(int index);

    void on_chapNhan_clicked();

    void on_huyBo_clicked();

private:
    Ui::FineDialog *ui;
    QSqlDatabase db;
    QModelIndexList selected;
    Inbox *inbox;
};

#endif // FINEDIALOG_H
