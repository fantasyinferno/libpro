#ifndef MANAGERGUI_H
#define MANAGERGUI_H

#include <QMainWindow>

namespace Ui {
class ManagerGUI;
}

class ManagerGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManagerGUI(QWidget *parent = 0);
    ~ManagerGUI();

private:
    Ui::ManagerGUI *ui;
};

#endif // MANAGERGUI_H
