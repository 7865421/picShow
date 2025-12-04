#include "scancodedialog.h"
#include "ui_scancode.h"
#include <QMessageBox>
#include <QDebug>
ScDialog::ScDialog(QWidget *parent) :
    QDialog(parent),
    scui(new Ui::ScDialog)
{
    scui->setupUi(this);
}

ScDialog::~ScDialog()
{
    delete scui;
}

void ScDialog::on_btlogin_clicked()
{
    //if(scui->textEdit->toPlainText() == tr("123456"))
      {
         accept();
      }
    //else {
       //   QMessageBox::warning(this, tr("警告！"),tr("密码错误！"),
      //          QMessageBox::Yes);
          // 清空内容并定位光标
         // ui->lePassWd->clear();
         // ui->lePassWd->setFocus();
   //   }


}

void ScDialog::on_btexit_clicked()
{
    this->close();
}

void ScDialog::set_text(QString txt)
{
   // scui->textEdit->setReadOnly(false);
    scui->textEdit->clear();
    scui->textEdit->setText(txt);
}
void ScDialog::set_lab(QString txt)
{
    scui->lbsc->clear();
    scui->lbsc->setText(txt);
}

void ScDialog::clear_text()
{
    scui->textEdit->clear();
   // scui->textEdit->setReadOnly(true);

}
QString ScDialog::get_text()
{
    return scui->textEdit->toPlainText();

}



void ScDialog::on_chbInput1_clicked()
{
   // scui->chbInput1->toggle();
    if(scui->chbInput1->checkState()==Qt::Checked)
        input_CH1=true;
    else
        input_CH1=false;
    qDebug()<<input_CH1;
}

void ScDialog::on_chbInput2_clicked()
{
   // scui->chbInput2->toggle();
    if(scui->chbInput2->checkState()==Qt::Checked)
        input_CH2=true;
    else
        input_CH2=false;
    qDebug()<<input_CH2;
}
