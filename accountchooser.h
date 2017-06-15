#ifndef ACCOUNTCHOOSER_H
#define ACCOUNTCHOOSER_H

#include <QDialog>

namespace Ui {
class AccountChooser;
}

class AccountChooser : public QDialog
{
    Q_OBJECT

public:
    explicit AccountChooser(QWidget *parent = 0);
    ~AccountChooser();

private:
    Ui::AccountChooser *ui;
};

#endif // ACCOUNTCHOOSER_H
