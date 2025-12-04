#ifndef SCLOGINDIALOG_H
#define SCLOGINDIALOG_H

#include <QDialog>

namespace Ui {
class ScDialog;
}

class ScDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScDialog(QWidget *parent = nullptr);
    ~ScDialog();
    void set_text(QString txt);
    void set_lab(QString txt);
    void clear_text();
    QString get_text();
    bool input_CH1=false;
    bool input_CH2=false;
private slots:
    void on_btlogin_clicked();

    void on_btexit_clicked();


    void on_chbInput1_clicked();

    void on_chbInput2_clicked();

private:
    Ui::ScDialog *scui;
};

#endif // LOGINDIALOG_H
