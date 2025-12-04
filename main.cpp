#include "mainwindow.h"
#include "logindialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

   // Dialog logindlg;


      //  if (logindlg.exec() == QDialog::Accepted)
      //  {
            w.setWindowState(Qt::WindowMaximized);
            w.show();
            return a.exec();
       // }
     //   else return 0;

}
