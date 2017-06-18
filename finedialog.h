#ifndef FINEDIALOG_H
#define FINEDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QModelIndexList>
namespace Ui {
class FineDialog;
}

class FineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FineDialog(QWidget *parent = 0, QSqlDatabase database = QSqlDatabase(), QModelIndexList list = QModelIndexList());
    ~FineDialog();

private slots:
    void on_hinhThuc_currentIndexChanged(int index);

private:
    Ui::FineDialog *ui;
    QSqlDatabase db;
    QModelIndexList selected;
};

#endif // FINEDIALOG_H
