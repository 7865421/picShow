#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mcprotocol/melsec_mc_bin.h"
#ifdef _WIN32
#include <WinSock2.h>
#endif
#include <QDebug>
#include<QFileDialog>



void MainWindow::on_btBrowseBaseDir1_clicked()
{
    QString FilePath=QFileDialog::getExistingDirectory(this,"请选择文件路径..");
    if(!FilePath.isEmpty())
        ui->lePicBaseDir1->setText(FilePath);
}

void MainWindow::on_btBrowseBaseDir2_clicked()
{
    QString FilePath=QFileDialog::getExistingDirectory(this,"请选择文件路径..");
    if(!FilePath.isEmpty())
        ui->lePicBaseDir2->setText(FilePath);
}

void MainWindow::on_btBrowseBaseDir3_clicked()
{
    QString FilePath=QFileDialog::getExistingDirectory(this,"请选择文件路径..");
    if(!FilePath.isEmpty())
        ui->lePicBaseDir3->setText(FilePath);
}






void MainWindow::on_lePLCip_textChanged(const QString &arg1)
{
    m_IniFile->setValue("PLCIP",ui->lePLCip->text());
   // ui->lePLCip->setText(ui->lePLCip->text());
}

void MainWindow::on_lePLCport_textChanged(const QString &arg1)
{
    m_IniFile->setValue("PLCPORT",ui->lePLCport->text());
   // ui->lePLCport->setText(ui->lePLCport->text());
}

void MainWindow::on_leTrigerAddr_textChanged(const QString &arg1)
{
    m_IniFile->setValue("TrigerAddr",ui->leTrigerAddr->text());
}


void MainWindow::on_leIDaddr_textChanged(const QString &arg1)
{
    m_IniFile->setValue("IDAddr",ui->leIDaddr->text());
}

void MainWindow::on_leIDmax_textChanged(const QString &arg1)
{
    m_IniFile->setValue("IDLength",ui->leIDmax->text());
}



void MainWindow::on_lePicBaseDir1_textChanged(const QString &arg1)
{
    m_IniFile->setValue("PicBaseDir1",ui->lePicBaseDir1->text());
}

void MainWindow::on_lePicBaseDir2_textChanged(const QString &arg1)
{
    m_IniFile->setValue("PicBaseDir2",ui->lePicBaseDir2->text());
}

void MainWindow::on_lePicBaseDir3_textChanged(const QString &arg1)
{
    m_IniFile->setValue("PicBaseDir3",ui->lePicBaseDir3->text());
}

void MainWindow::on_leDevType1_textChanged(const QString &arg1)
{
    m_IniFile->setValue("DevType1",ui->leDevType1->text());
}

void MainWindow::on_leDevType2_textChanged(const QString &arg1)
{
    m_IniFile->setValue("DevType2",ui->leDevType2->text());
}

void MainWindow::on_leDevType3_textChanged(const QString &arg1)
{
    m_IniFile->setValue("DevType3",ui->leDevType3->text());
}

void MainWindow::on_leDirBack1_textChanged(const QString &arg1)
{
    m_IniFile->setValue("DirBack1",ui->leDirBack1->text());
}

void MainWindow::on_leDirBack2_textChanged(const QString &arg1)
{
    m_IniFile->setValue("DirBack2",ui->leDirBack2->text());
}

void MainWindow::on_leDirBack3_textChanged(const QString &arg1)
{
    m_IniFile->setValue("DirBack3",ui->leDirBack3->text());
}


void MainWindow::on_leDevTypeAddr1_textChanged(const QString &arg1)
{
    m_IniFile->setValue("TypeAddr1",ui->leDevTypeAddr1->text());
}

void MainWindow::on_leDevTypeAddr2_textChanged(const QString &arg1)
{
    m_IniFile->setValue("TypeAddr2",ui->leDevTypeAddr2->text());
}

void MainWindow::on_leDevTypeAddr3_textChanged(const QString &arg1)
{
    m_IniFile->setValue("TypeAddr3",ui->leDevTypeAddr3->text());
}

void MainWindow::on_leTypeLenth_textChanged(const QString &arg1)
{
    m_IniFile->setValue("TypeLength",ui->leTypeLenth->text());
}


void MainWindow::on_chbAutoType_stateChanged(int arg1)
{
    if(ui->chbAutoType->checkState()==Qt::Checked)
    {
        m_IniFile->setValue("AutoType","1");
    }
    else
    {
         m_IniFile->setValue("AutoType","0");
    }
}


void MainWindow::on_chbAddDate_stateChanged(int arg1)
{

    if(ui->chbAddDate->checkState()==Qt::Checked)
    {
        m_IniFile->setValue("AddDate","1");
    }
    else
    {
         m_IniFile->setValue("AddDate","0");
    }

}

void MainWindow::on_chbAutoRead_stateChanged(int arg1)
{
    if(ui->chbAutoRead->checkState()==Qt::Checked)
    {
        m_IniFile->setValue("OpenByID","1");
    }
    else
    {
         m_IniFile->setValue("OpenByID","0");
    }
}

void MainWindow::on_chbAutoLogOn_stateChanged(int arg1)
{
    if(ui->chbAutoLogOn->checkState()==Qt::Checked)
    {
        m_IniFile->setValue("AutoLogOn","1");
    }
    else
    {
         m_IniFile->setValue("AutoLogOn","0");
    }
}

void MainWindow::on_leScMax_textChanged(const QString &arg1)
{
     m_IniFile->setValue("ScMaxLength",ui->leScMax->text());
}



void MainWindow::on_leScWrAddr_textChanged(const QString &arg1)
{
    m_IniFile->setValue("ScWrteAddr",ui->leScWrAddr->text());
}

void MainWindow::on_leScWrAddr2_textChanged(const QString &arg1)
{
    m_IniFile->setValue("ScWrteAddr2",ui->leScWrAddr2->text());
}

void MainWindow::on_leScFlagAddr_textChanged(const QString &arg1)
{
     m_IniFile->setValue("ScFlagAddr",ui->leScFlagAddr->text());
}

void MainWindow::on_leScFlagAddr2_textChanged(const QString &arg1)
{
     m_IniFile->setValue("ScFlagAddr2",ui->leScFlagAddr2->text());
}

void MainWindow::on_leInputTriger_textChanged(const QString &arg1)
{
     m_IniFile->setValue("ManualInputAddr",ui->leInputTriger->text());
}


void MainWindow::on_leTransAngle_textChanged(const QString &arg1)
{
     m_IniFile->setValue("TransAngle",ui->leTransAngle->text());
}
