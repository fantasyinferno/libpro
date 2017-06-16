#ifndef INFORMATION_H
#define INFORMATION_H

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QItemSelection>
#include <QDataWidgetMapper>
#include <QSqlRelationalTableModel>
#include <QList>

namespace Ui {
class Information;
}

class Information : public QDialog
{
    Q_OBJECT

public:
    explicit Information(QWidget *parent = 0, QSqlDatabase database = QSqlDatabase());
    ~Information();
    void load(QString, QString);
    void submitVt();
    void checkVt();
    void enableEdit(bool);
    void submitAv();

private slots:
    void on_hoanTatButton_clicked();
    void on_thayDoiButton_clicked();
    void on_dangNhapThanhCong(int, QString);
    void on_updateMyBooks(const QModelIndexList& selectedList);
    void on_huyButton_clicked();
    void on_avatarButton_clicked();
    void on_currentIndexChanged(int);
    void on_informationRequest();
    void on_dangXuat();
    void on_iAmYourParent(QWidget*);
signals:
    void avatarChanged(const QPixmap* pixmap);
    void rolesLoaded(QList<int>);
private:
    QSqlDatabase db;
    Ui::Information *ui;
    QSqlRelationalTableModel *model, *bookModel;
    QDataWidgetMapper *mapper;
    QString user;
    QList<int> rolesList;
    int user_id;
};

#endif // INFORMATION_H
