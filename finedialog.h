#ifndef FINEDIALOG_H
#define FINEDIALOG_H

#include <QDialog>

namespace Ui {
class FineDialog;
}

class FineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FineDialog(QWidget *parent = 0);
    ~FineDialog();

private:
    Ui::FineDialog *ui;
};

#endif // FINEDIALOG_H
