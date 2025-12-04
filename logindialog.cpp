#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_btlogin_clicked()
{
    if(ui->lePassWd->text() == tr("123456"))
      {
         accept();
      } else {
          QMessageBox::warning(this, tr("警告！"),tr("密码错误！"),
                QMessageBox::Yes);
          // 清空内容并定位光标
          ui->lePassWd->clear();
          ui->lePassWd->setFocus();
      }


}

void Dialog::on_btexit_clicked()
{
    this->close();
}
