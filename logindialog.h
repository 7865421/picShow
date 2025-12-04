#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_btlogin_clicked();

    void on_btexit_clicked();

private:
    Ui::Dialog *ui;
};

#endif // LOGINDIALOG_H
