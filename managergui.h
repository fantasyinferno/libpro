#ifndef MANAGERGUI_H
#define MANAGERGUI_H

#include <QMainWindow>
#include "information.h"
#include "introform.h"

#include <QDialog>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QItemSelection>
#include <QDataWidgetMapper>
#include <QSqlRelationalTableModel>

namespace Ui {
class ManagerGUI;
}

class ManagerGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManagerGUI(QWidget *parent = 0);
    void initializeTable();
    ~ManagerGUI();


private slots:
    void submitVt();
    void checkVt();
    void on_thayDoiButton_clicked();

    void on_xoaButton_clicked();

    void on_thanhTimKiem_returnPressed();

    void on_timKiemButton_clicked();

private:
    QSqlRelationalTableModel *model;
    Information *information;
    IntroForm *introform;
    QDataWidgetMapper *mapper;

    QSqlDatabase db;

    Ui::ManagerGUI *ui;
};

#endif // MANAGERGUI_H
