#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mcprotocol/melsec_mc_bin.h"
#ifdef _WIN32
#include <WinSock2.h>
#endif
#include <QDebug>
#include<QFileDialog>
#include<QIcon>
#include <QMetaEnum>
#include<QKeyEvent>
#include<QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/icon.jpg"));
    this->installEventFilter(this);
    StartInit();
}

MainWindow::~MainWindow()
{
    delete ui;
#ifdef _WIN32
    WSACleanup();
#endif
}

void MainWindow::StartInit()
{
    config_init();

    createActions();
    createMenus();

    scene1 = new DiagramScene(itemMenu, this);
    scene2 = new DiagramScene(itemMenu, this);
    scene3 = new DiagramScene(itemMenu, this);
    scene4 = new DiagramScene(itemMenu, this);
    scene5 = new DiagramScene(itemMenu, this);
    scene6 = new DiagramScene(itemMenu, this);

    ui->graphicsView1->setScene(scene1);
    ui->graphicsView1->show();
    ui->graphicsView2->setScene(scene2);
    ui->graphicsView2->show();
    ui->graphicsView3->setScene(scene3);
    ui->graphicsView3->show();
    ui->graphicsView4->setScene(scene4);
    ui->graphicsView4->show();
    ui->graphicsView5->setScene(scene5);
    ui->graphicsView5->show();
    ui->graphicsView6->setScene(scene6);
    ui->graphicsView6->show();

    connect(scene1, SIGNAL(scaleChanging(int)),
            this, SLOT(scene1ScaleZooming(int)));
    connect(scene2, SIGNAL(scaleChanging(int)),
            this, SLOT(scene2ScaleZooming(int)));
    connect(scene3, SIGNAL(scaleChanging(int)),
            this, SLOT(scene3ScaleZooming(int)));
    connect(scene4, SIGNAL(scaleChanging(int)),
            this, SLOT(scene4ScaleZooming(int)));
    connect(scene5, SIGNAL(scaleChanging(int)),
            this, SLOT(scene5ScaleZooming(int)));
    connect(scene6, SIGNAL(scaleChanging(int)),
            this, SLOT(scene6ScaleZooming(int)));


    qDebug()<<"开始初始化socket环境";
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return;
    }
#endif
qDebug()<<"初始化socket环境完成";
QString FilenamePic1,FilenamePic2,FilenamePic3,FilenamePic4,FilenamePic5,FilenamePic6;
IDString="E02-X3A3151FJ-0142";
DateString= QDateTime::currentDateTime().toString("yyyy-MM-dd");
FilenamePic1=m_IniFile->value("PicBaseDir1").toString()
        +"/"+DateString
        +"/"+m_IniFile->value("DevType1").toString()
        +"/"+IDString+"/CAM-0/"+m_IniFile->value("DirBack1").toString()
        +"/"+IDString+".jpeg";
FilenamePic2=m_IniFile->value("PicBaseDir1").toString()
        +"/"+DateString
        +"/"+m_IniFile->value("DevType1").toString()
        +"/"+IDString+"/CAM-1/"+m_IniFile->value("DirBack1").toString()
        +"/"+IDString+".jpeg";
FilenamePic3=m_IniFile->value("PicBaseDir2").toString()
        +"/"+DateString
        +"/"+m_IniFile->value("DevType2").toString()
        +"/"+IDString+"/CAM-0/"+m_IniFile->value("DirBack2").toString()
        +"/"+IDString+".jpeg";
FilenamePic4=m_IniFile->value("PicBaseDir2").toString()
        +"/"+DateString
        +"/"+m_IniFile->value("DevType2").toString()
        +"/"+IDString+"/CAM-1/"+m_IniFile->value("DirBack2").toString()
        +"/"+IDString+".jpeg";
FilenamePic5=m_IniFile->value("PicBaseDir3").toString()
        +"/"+DateString
        +"/"+m_IniFile->value("DevType3").toString()
        +"/"+IDString+"/CAM-0/"+m_IniFile->value("DirBack3").toString()
        +"/"+IDString+".jpeg";
FilenamePic6=m_IniFile->value("PicBaseDir3").toString()
        +"/"+DateString
        +"/"+m_IniFile->value("DevType3").toString()
        +"/"+IDString+"/CAM-1/"+m_IniFile->value("DirBack3").toString()
        +"/"+IDString+".jpeg";

ui->lbFilePic1->setText(FilenamePic1);
ui->lbFilePic2->setText(FilenamePic2);
ui->lbFilePic3->setText(FilenamePic3);
ui->lbFilePic4->setText(FilenamePic4);
ui->lbFilePic5->setText(FilenamePic5);
ui->lbFilePic6->setText(FilenamePic6);


ui->leScMax->setText(m_IniFile->value("ScMaxLength").toString());
ui->leScWrAddr->setText(m_IniFile->value("ScWrteAddr").toString());
ui->leScFlagAddr->setText(m_IniFile->value("ScFlagAddr").toString());
ui->leScWrAddr2->setText(m_IniFile->value("ScWrteAddr2").toString());
ui->leScFlagAddr2->setText(m_IniFile->value("ScFlagAddr2").toString());
ui->leInputTriger->setText(m_IniFile->value("ManualInputAddr").toString());
ui->leTransAngle->setText(m_IniFile->value("TransAngle").toString());

/*
    ui->lbFilePic1->setText(m_IniFile->value("PicBaseDir1").toString());
    ui->lbFilePic2->setText(m_IniFile->value("PicBaseDir1").toString());
    ui->lbFilePic3->setText(m_IniFile->value("PicBaseDir2").toString());
    ui->lbFilePic4->setText(m_IniFile->value("PicBaseDir2").toString());
    ui->lbFilePic5->setText(m_IniFile->value("PicBaseDir3").toString());
    ui->lbFilePic6->setText(m_IniFile->value("PicBaseDir3").toString());
*/


    plcConn  = new Connection(this);

    connect(plcConn,&Connection::ConnectDone,this,&MainWindow::plcConnectDone);

    PLCreadTimer= new QTimer(this);
    connect(PLCreadTimer, SIGNAL(timeout()), this, SLOT(plcRead()));

    connect(this, SIGNAL(get_scancode(QString)), this, SLOT(show_scancode(QString)));

    PLCreadTimer->start(500);//500毫秒

    ui->tabWidget->setCurrentIndex(0);

    ui->tab2->setEnabled(false);

//keycap

    //bool ret = GlobalKeyEvent::installKeyEvent();
  //  connect(GlobalKeyEvent::getInstance(), &GlobalKeyEvent::keyEvent, this, &MainWindow::on_keyEvent);

}

bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
    if(QEvent::KeyPress == e->type())
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);

            sc_cnt++;
              if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
            {
               // qDebug()<<mystr;
                if(!g_ScanCode.isEmpty()){
                    emit get_scancode(g_ScanCode);
                    g_ScanCode.clear();
                   }

            }
            else
            {
                g_ScanCode += keyEvent->text();
              //  qDebug()<<"******"<<keyEvent->text();

            }

      //  qDebug()<<"Window Qt pressed keyevent";
        }
    return QMainWindow::eventFilter(obj, e);
}



void MainWindow::on_keyEvent(QKeyEvent* event)
{
   // static QString mystr;
    QMetaEnum type = QMetaEnum::fromType<QEvent::Type>();
    QMetaEnum key = QMetaEnum::fromType<Qt::Key>();
    QMetaEnum keyboard = QMetaEnum::fromType<Qt::KeyboardModifiers>();
   // QString str = QString("状态：[%1]\t按键：[%2]\t修饰：[%3]]").arg(type.valueToKey(event->type()))
   //                                        .arg(key.valueToKey(event->key()))
   //                                        .arg(QString(keyboard.valueToKeys(int(event->modifiers()))));
    if(event->type()==QEvent::KeyPress)
    {
        sc_cnt++;
        if(event->text()=="\r")
        {
           // qDebug()<<mystr;
            if(!g_ScanCode.isEmpty()){
                emit get_scancode(g_ScanCode);
                g_ScanCode.clear();
               }

        }
        else
        {   if(!event->text().isEmpty())
           {  g_ScanCode += QString(event->text());
                qDebug()<<"******"<<QString(event->text());
           }
        }
    }

    delete event;       // 使用完成后记得delete
}
void MainWindow::on_btOpenPic1_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("打开第一台CAM0图片"),m_IniFile->value("PicBaseDir1").toString(),tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
       if (fileName.isEmpty())
       {
           return;
       }

       QImage imgfile(fileName);
       QImage images(imgfile);
       QMatrix matrix;
       matrix.rotate(m_IniFile->value("TransAngle").toString().toFloat());//逆时针旋转90度
       images= images.transformed(matrix,Qt::FastTransformation);
       scene1->clear();
      // scene1->backgroundPixmap =QPixmap::fromImage(QImage(fileName));
       scene1->backgroundPixmap =QPixmap::fromImage(images);
       QGraphicsPixmapItem* _backGroundItem= scene1->addPixmap(scene1->backgroundPixmap);
       QRect mainRect(0,0,scene1->backgroundPixmap.width(),scene1->backgroundPixmap.height());
       scene1->setSceneRect(mainRect);//设置场景范围
       _backGroundItem->setPos(0,0);


       ui->graphicsView1->fitInView(_backGroundItem,Qt::KeepAspectRatio);

       ui->lbFilePic1->setText(fileName);

}



void MainWindow::on_btOpenPic2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("打开第一台CAM1图片"),m_IniFile->value("PicBaseDir2").toString(),tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
       if (fileName.isEmpty())
       {
           return;
       }
       QImage imgfile(fileName);
       QImage images(imgfile);
       QMatrix matrix;
       matrix.rotate(m_IniFile->value("TransAngle").toString().toFloat());//逆时针旋转90度
       images= images.transformed(matrix,Qt::FastTransformation);
       scene2->clear();
      // scene2->backgroundPixmap =QPixmap::fromImage(QImage(fileName));
       scene2->backgroundPixmap =QPixmap::fromImage(images);
       QGraphicsPixmapItem* _backGroundItem= scene2->addPixmap(scene2->backgroundPixmap);
       QRect mainRect(0,0,scene2->backgroundPixmap.width(),scene2->backgroundPixmap.height());
       scene2->setSceneRect(mainRect);//设置场景范围
       _backGroundItem->setPos(0,0);
       ui->graphicsView2->fitInView(_backGroundItem,Qt::KeepAspectRatio);

       ui->lbFilePic2->setText(fileName);

}

void MainWindow::on_btOpenPic3_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("打开第二台CAM0图片"),m_IniFile->value("PicBaseDir3").toString(),tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
       if (fileName.isEmpty())
       {
           return;
       }
       QImage imgfile(fileName);
       QImage images(imgfile);
       QMatrix matrix;
       matrix.rotate(m_IniFile->value("TransAngle").toString().toFloat());//逆时针旋转90度
       images= images.transformed(matrix,Qt::FastTransformation);

       scene3->clear();
      // scene3->backgroundPixmap =QPixmap::fromImage(QImage(fileName));
       scene3->backgroundPixmap =QPixmap::fromImage(images);
       QGraphicsPixmapItem* _backGroundItem= scene3->addPixmap(scene3->backgroundPixmap);
       QRect mainRect(0,0,scene3->backgroundPixmap.width(),scene3->backgroundPixmap.height());
       scene3->setSceneRect(mainRect);//设置场景范围
       _backGroundItem->setPos(0,0);
       ui->graphicsView3->fitInView(_backGroundItem,Qt::KeepAspectRatio);

       ui->lbFilePic3->setText(fileName);

}

void MainWindow::on_btOpenPic4_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("打开第二台CAM1图片"),m_IniFile->value("PicBaseDir4").toString(),tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
       if (fileName.isEmpty())
       {
           return;
       }
       QImage imgfile(fileName);
       QImage images(imgfile);
       QMatrix matrix;
       matrix.rotate(m_IniFile->value("TransAngle").toString().toFloat());//逆时针旋转90度
       images= images.transformed(matrix,Qt::FastTransformation);

       scene4->clear();
       //scene4->backgroundPixmap =QPixmap::fromImage(QImage(fileName));
       scene4->backgroundPixmap =QPixmap::fromImage(images);
       QGraphicsPixmapItem* _backGroundItem= scene4->addPixmap(scene4->backgroundPixmap);
       QRect mainRect(0,0,scene4->backgroundPixmap.width(),scene4->backgroundPixmap.height());
       scene4->setSceneRect(mainRect);//设置场景范围
       _backGroundItem->setPos(0,0);
       ui->graphicsView4->fitInView(_backGroundItem,Qt::KeepAspectRatio);

       ui->lbFilePic4->setText(fileName);

}

void MainWindow::on_btOpenPic5_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("打开第三台CAM0图片"),m_IniFile->value("PicBaseDir5").toString(),tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
       if (fileName.isEmpty())
       {
           return;
       }
       QImage imgfile(fileName);
       QImage images(imgfile);
       QMatrix matrix;
       matrix.rotate(m_IniFile->value("TransAngle").toString().toFloat());//逆时针旋转90度
       images= images.transformed(matrix,Qt::FastTransformation);

       scene5->clear();
       //scene5->backgroundPixmap =QPixmap::fromImage(QImage(fileName));
       scene5->backgroundPixmap =QPixmap::fromImage(images);
       QGraphicsPixmapItem* _backGroundItem= scene5->addPixmap(scene5->backgroundPixmap);
       QRect mainRect(0,0,scene5->backgroundPixmap.width(),scene5->backgroundPixmap.height());
       scene5->setSceneRect(mainRect);//设置场景范围
       _backGroundItem->setPos(0,0);
       ui->graphicsView5->fitInView(_backGroundItem,Qt::KeepAspectRatio);

       ui->lbFilePic5->setText(fileName);

}

void MainWindow::on_btOpenPic6_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("打开第三台CAM1图片"),m_IniFile->value("PicBaseDir6").toString(),tr("Image File(*.png *.jpg *.jpeg *.bmp)"));
       if (fileName.isEmpty())
       {
           return;
       }

       QImage imgfile(fileName);
       QImage images(imgfile);
       QMatrix matrix;
       matrix.rotate(m_IniFile->value("TransAngle").toString().toFloat());//逆时针旋转90度
       images= images.transformed(matrix,Qt::FastTransformation);

       scene6->clear();
      // scene6->backgroundPixmap =QPixmap::fromImage(QImage(fileName));
       scene6->backgroundPixmap =QPixmap::fromImage(images);
       QGraphicsPixmapItem* _backGroundItem= scene6->addPixmap(scene6->backgroundPixmap);
       QRect mainRect(0,0,scene6->backgroundPixmap.width(),scene6->backgroundPixmap.height());
       scene6->setSceneRect(mainRect);//设置场景范围
       _backGroundItem->setPos(0,0);
       ui->graphicsView6->fitInView(_backGroundItem,Qt::KeepAspectRatio);

       ui->lbFilePic6->setText(fileName);

}

void MainWindow::plcConnectDone(int fd)
{
    socketfd=fd;
    qDebug()<<"获取连接完成信号"<<socketfd;
    if(socketfd>0){
            ui->btConnect->setText("断开");
            ui->lbConnectStatus->setText("已连接");
            qDebug()<<"连接成功";
            conStatus=true;
/*
    char* str_val = NULL;
    qDebug()<<"读取数据返回"<< mc_read_string(socketfd, "D100", 20, &str_val);
     printf("Read\t D100 \tstring:\t %s\n", str_val);
     free(str_val);
*/
    }
    else
    qDebug()<<"连接失败";
}


void MainWindow::on_btConnect_clicked()
{

    if(ui->btConnect->text()=="连接"){
    plcConn->myPLCip=ui->lePLCip->text();
    plcConn->myPLCport=ui->lePLCport->text().toInt();
    plcConn->start();
    }
    else
    {
        mc_disconnect(socketfd);
        ui->btConnect->setText("连接");
        socketfd=-1;
        ui->lbConnectStatus->setText("未连接");
        conStatus=false;
    }
}
//读取PLC定时器，500ms一次
void MainWindow::plcRead()
{
    static int cntM=0;
    static int cntS=0;
    static int reconCnt=0;
    static bool lastTriger=false;
    //sc
    static int last_cnt_sc=0;
    static int cntSC=0;

    static int flagcnt=0;
    static int flagcnt2=0;
    static int trigercnt=0;

    QString Type1,Type2,Type3;

    datetime=QDateTime::currentDateTime();
    if(ui->chbAddDate->checkState()==Qt::Checked)
        DateString=datetime.toString("yyyy-MM-dd");
    else
        DateString="";

    //qDebug()<<"当前日期"<<DateString;





    if(conStatus)//如果连接正常，开始读取触发值，读取ID值
    {
        char* str_val = NULL;

         if(mc_read_string(socketfd,  m_IniFile->value("IDAddr").toString().toLocal8Bit().data(), m_IniFile->value("IDLength").toInt(), &str_val))
         {//读取数据成功
             cntS=0;
             IDString=QString::fromLocal8Bit(str_val,m_IniFile->value("IDLength").toInt());
             ui->lbIDValue->setText(IDString);
            // qDebug()<<"当前ID"<<IDString;

         }
         else
         {
             cntS++;
             if(cntS==5)//5次读取都失败，说明断线了
               {//断开连接，等待重新链接
                 mc_disconnect(socketfd);
                 ui->btConnect->setText("连接");
                 socketfd=-1;
                 ui->lbConnectStatus->setText("未连接");
                 conStatus=false;
                 cntS=0;
                 cntM=0;
                }
         }
         free(str_val);


         char* str_dev_type1 = NULL;
         char* str_dev_type2 = NULL;
         char* str_dev_type3 = NULL;


        if( mc_read_string(socketfd,  m_IniFile->value("TypeAddr1").toString().toLocal8Bit().data(), m_IniFile->value("TypeLength").toInt(), &str_dev_type1))
            Type1=QString::fromLocal8Bit(str_dev_type1,m_IniFile->value("TypeLength").toInt());
         if(mc_read_string(socketfd,  m_IniFile->value("TypeAddr2").toString().toLocal8Bit().data(), m_IniFile->value("TypeLength").toInt(), &str_dev_type2))
            Type2=QString::fromLocal8Bit(str_dev_type1,m_IniFile->value("TypeLength").toInt());
         if(mc_read_string(socketfd,  m_IniFile->value("TypeAddr3").toString().toLocal8Bit().data(), m_IniFile->value("TypeLength").toInt(), &str_dev_type3))
            Type3=QString::fromLocal8Bit(str_dev_type1,m_IniFile->value("TypeLength").toInt());

         free(str_dev_type1);
         free(str_dev_type2);
         free(str_dev_type3);

         ui->lbDevType1->setText(Type1);
         ui->lbDevType2->setText(Type2);
         ui->lbDevType3->setText(Type3);

        bool val = false;

        if(mc_read_bool(socketfd, m_IniFile->value("TrigerAddr").toString().toLocal8Bit().data(), &val))//读取触发值成功
         {
              cntM=0;
              ui->lbTrigerValue->setText(QString::number(val));
              if((lastTriger!=val) && val)
              if(ui->chbAutoRead->checkState()==Qt::Checked)
              {//自动打开文件
                  QString FilenamePic1;
                  QString FilenamePic2;
                  QString FilenamePic3;
                  QString FilenamePic4;
                  QString FilenamePic5;
                  QString FilenamePic6;
                  if(ui->chbAddDate->checkState()==Qt::Checked){
                      if(ui->chbAutoType->checkState()==Qt::Checked)
                      {
                          FilenamePic1=m_IniFile->value("PicBaseDir1").toString()
                                  +"/"+DateString
                                  +"/"+Type1
                                  +"/"+IDString+"/CAM-0/"+m_IniFile->value("DirBack1").toString()
                                  +"/"+IDString+".jpeg";
                          FilenamePic2=m_IniFile->value("PicBaseDir1").toString()
                                  +"/"+DateString
                                  +"/"+Type1
                                  +"/"+IDString+"/CAM-1/"+m_IniFile->value("DirBack1").toString()
                                  +"/"+IDString+".jpeg";
                          FilenamePic3=m_IniFile->value("PicBaseDir2").toString()
                                  +"/"+DateString
                                  +"/"+Type2
                                  +"/"+IDString+"/CAM-0/"+m_IniFile->value("DirBack2").toString()
                                  +"/"+IDString+".jpeg";
                          FilenamePic4=m_IniFile->value("PicBaseDir2").toString()
                                  +"/"+DateString
                                  +"/"+Type2
                                  +"/"+IDString+"/CAM-1/"+m_IniFile->value("DirBack2").toString()
                                  +"/"+IDString+".jpeg";
                          FilenamePic5=m_IniFile->value("PicBaseDir3").toString()
                                  +"/"+DateString
                                  +"/"+Type3
                                  +"/"+IDString+"/CAM-0/"+m_IniFile->value("DirBack3").toString()
                                  +"/"+IDString+".jpeg";
                          FilenamePic6=m_IniFile->value("PicBaseDir3").toString()
                                  +"/"+DateString
                                  +"/"+Type3
                                  +"/"+IDString+"/CAM-1/"+m_IniFile->value("DirBack3").toString()
                                  +"/"+IDString+".jpeg";
                      }
                      else
                      {
                    FilenamePic1=m_IniFile->value("PicBaseDir1").toString()
                            +"/"+DateString
                            +"/"+m_IniFile->value("DevType1").toString()
                            +"/"+IDString+"/CAM-0/"+m_IniFile->value("DirBack1").toString()
                            +"/"+IDString+".jpeg";
                    FilenamePic2=m_IniFile->value("PicBaseDir1").toString()
                            +"/"+DateString
                            +"/"+m_IniFile->value("DevType1").toString()
                            +"/"+IDString+"/CAM-1/"+m_IniFile->value("DirBack1").toString()
                            +"/"+IDString+".jpeg";
                    FilenamePic3=m_IniFile->value("PicBaseDir2").toString()
                            +"/"+DateString
                            +"/"+m_IniFile->value("DevType2").toString()
                            +"/"+IDString+"/CAM-0/"+m_IniFile->value("DirBack2").toString()
                            +"/"+IDString+".jpeg";
                    FilenamePic4=m_IniFile->value("PicBaseDir2").toString()
                            +"/"+DateString
                            +"/"+m_IniFile->value("DevType2").toString()
                            +"/"+IDString+"/CAM-1/"+m_IniFile->value("DirBack2").toString()
                            +"/"+IDString+".jpeg";
                    FilenamePic5=m_IniFile->value("PicBaseDir3").toString()
                            +"/"+DateString
                            +"/"+m_IniFile->value("DevType3").toString()
                            +"/"+IDString+"/CAM-0/"+m_IniFile->value("DirBack3").toString()
                            +"/"+IDString+".jpeg";
                    FilenamePic6=m_IniFile->value("PicBaseDir3").toString()
                            +"/"+DateString
                            +"/"+m_IniFile->value("DevType3").toString()
                            +"/"+IDString+"/CAM-1/"+m_IniFile->value("DirBack3").toString()
                            +"/"+IDString+".jpeg";
                      }
                  }
                  else{
                      FilenamePic1=m_IniFile->value("PicBaseDir1").toString()
                              +"/"+m_IniFile->value("DevType1").toString()
                              +"/"+IDString+"/CAM-0/"+m_IniFile->value("DirBack1").toString()
                              +"/"+IDString+".jpeg";
                      FilenamePic2=m_IniFile->value("PicBaseDir1").toString()
                              +"/"+m_IniFile->value("DevType1").toString()
                              +"/"+IDString+"/CAM-1/"+m_IniFile->value("DirBack1").toString()
                              +"/"+IDString+".jpeg";
                      FilenamePic3=m_IniFile->value("PicBaseDir2").toString()
                              +"/"+m_IniFile->value("DevType2").toString()
                              +"/"+IDString+"/CAM-0/"+m_IniFile->value("DirBack2").toString()
                              +"/"+IDString+".jpeg";
                      FilenamePic4=m_IniFile->value("PicBaseDir2").toString()
                              +"/"+m_IniFile->value("DevType2").toString()
                              +"/"+IDString+"/CAM-1/"+m_IniFile->value("DirBack2").toString()
                              +"/"+IDString+".jpeg";
                      FilenamePic5=m_IniFile->value("PicBaseDir3").toString()
                              +"/"+m_IniFile->value("DevType3").toString()
                              +"/"+IDString+"/CAM-0/"+m_IniFile->value("DirBack3").toString()
                              +"/"+IDString+".jpeg";
                      FilenamePic6=m_IniFile->value("PicBaseDir3").toString()
                              +"/"+m_IniFile->value("DevType3").toString()
                              +"/"+IDString+"/CAM-1/"+m_IniFile->value("DirBack3").toString()
                              +"/"+IDString+".jpeg";
                  }
                  ui->lbFilePic1->setText(FilenamePic1);
                  ui->lbFilePic2->setText(FilenamePic2);
                  ui->lbFilePic3->setText(FilenamePic3);
                  ui->lbFilePic4->setText(FilenamePic4);
                  ui->lbFilePic5->setText(FilenamePic5);
                  ui->lbFilePic6->setText(FilenamePic6);

                  QImage imgfile(FilenamePic1);
                  QImage images(imgfile);
                  QMatrix matrix;

                  matrix.rotate(m_IniFile->value("TransAngle").toString().toFloat());//逆时针旋转90度
                  images= images.transformed(matrix,Qt::FastTransformation);


                  scene1->clear();
                 // scene1->backgroundPixmap =QPixmap::fromImage(QImage(FilenamePic1));
                  scene1->backgroundPixmap =QPixmap::fromImage(images);
                  QGraphicsPixmapItem* _backGroundItem1= scene1->addPixmap(scene1->backgroundPixmap);
                  QRect mainRect1(0,0,scene1->backgroundPixmap.width(),scene1->backgroundPixmap.height());
                  scene1->setSceneRect(mainRect1);//设置场景范围
                  _backGroundItem1->setPos(0,0);
                  ui->graphicsView1->fitInView(_backGroundItem1,Qt::KeepAspectRatio);

                  QImage imgfile1(FilenamePic2);
                  QImage images1(imgfile1);
                  QMatrix matrix1;
                  matrix1.rotate(m_IniFile->value("TransAngle").toString().toFloat());//逆时针旋转90度
                  images1= images1.transformed(matrix1,Qt::FastTransformation);

                  scene2->clear();
                 // scene2->backgroundPixmap =QPixmap::fromImage(QImage(FilenamePic2));
                  scene2->backgroundPixmap =QPixmap::fromImage(images1);
                  QGraphicsPixmapItem* _backGroundItem2= scene2->addPixmap(scene2->backgroundPixmap);
                  QRect mainRect2(0,0,scene2->backgroundPixmap.width(),scene2->backgroundPixmap.height());
                  scene2->setSceneRect(mainRect2);//设置场景范围
                  _backGroundItem2->setPos(0,0);
                  ui->graphicsView2->fitInView(_backGroundItem2,Qt::KeepAspectRatio);

                  QImage imgfile2(FilenamePic3);
                  QImage images2(imgfile2);
                  QMatrix matrix2;
                  matrix2.rotate(m_IniFile->value("TransAngle").toString().toFloat());//逆时针旋转90度
                  images2= images2.transformed(matrix2,Qt::FastTransformation);

                  scene3->clear();
                 // scene3->backgroundPixmap =QPixmap::fromImage(QImage(FilenamePic3));
                  scene3->backgroundPixmap =QPixmap::fromImage(images2);
                  QGraphicsPixmapItem* _backGroundItem3= scene3->addPixmap(scene3->backgroundPixmap);
                  QRect mainRect3(0,0,scene3->backgroundPixmap.width(),scene3->backgroundPixmap.height());
                  scene3->setSceneRect(mainRect3);//设置场景范围
                  _backGroundItem3->setPos(0,0);
                  ui->graphicsView3->fitInView(_backGroundItem3,Qt::KeepAspectRatio);

                  QImage imgfile3(FilenamePic4);
                  QImage images3(imgfile3);
                  QMatrix matrix3;
                  matrix3.rotate(m_IniFile->value("TransAngle").toString().toFloat());//逆时针旋转90度
                  images3= images3.transformed(matrix3,Qt::FastTransformation);

                  scene4->clear();
                 // scene4->backgroundPixmap =QPixmap::fromImage(QImage(FilenamePic4));
                  scene4->backgroundPixmap =QPixmap::fromImage(images3);
                  QGraphicsPixmapItem* _backGroundItem4= scene4->addPixmap(scene4->backgroundPixmap);
                  QRect mainRect4(0,0,scene4->backgroundPixmap.width(),scene4->backgroundPixmap.height());
                  scene4->setSceneRect(mainRect4);//设置场景范围
                  _backGroundItem4->setPos(0,0);
                  ui->graphicsView4->fitInView(_backGroundItem4,Qt::KeepAspectRatio);

                  QImage imgfile4(FilenamePic5);
                  QImage images4(imgfile4);
                  QMatrix matrix4;
                  matrix4.rotate(m_IniFile->value("TransAngle").toString().toFloat());//逆时针旋转90度
                  images4= images4.transformed(matrix4,Qt::FastTransformation);

                  scene5->clear();
                 // scene5->backgroundPixmap =QPixmap::fromImage(QImage(FilenamePic5));
                  scene5->backgroundPixmap =QPixmap::fromImage(images4);
                  QGraphicsPixmapItem* _backGroundItem5= scene5->addPixmap(scene5->backgroundPixmap);
                  QRect mainRect5(0,0,scene5->backgroundPixmap.width(),scene5->backgroundPixmap.height());
                  scene5->setSceneRect(mainRect5);//设置场景范围
                  _backGroundItem5->setPos(0,0);
                  ui->graphicsView5->fitInView(_backGroundItem5,Qt::KeepAspectRatio);

                  QImage imgfile5(FilenamePic6);
                  QImage images5(imgfile5);
                  QMatrix matrix5;
                  matrix5.rotate(m_IniFile->value("TransAngle").toString().toFloat());//逆时针旋转90度
                  images5= images5.transformed(matrix5,Qt::FastTransformation);
                  scene6->clear();
                  //scene6->backgroundPixmap =QPixmap::fromImage(QImage(FilenamePic6));
                  scene6->backgroundPixmap =QPixmap::fromImage(images5);
                  QGraphicsPixmapItem* _backGroundItem6= scene6->addPixmap(scene6->backgroundPixmap);
                  QRect mainRect6(0,0,scene6->backgroundPixmap.width(),scene6->backgroundPixmap.height());
                  scene6->setSceneRect(mainRect6);//设置场景范围
                  _backGroundItem6->setPos(0,0);
                  ui->graphicsView6->fitInView(_backGroundItem6,Qt::KeepAspectRatio);

              }

         }
         else
         {
              cntM++;
              if(cntM==5)//5次读取都失败，说明断线了
                {//断开连接，等待重新链接
                  mc_disconnect(socketfd);
                  ui->btConnect->setText("连接");
                  socketfd=-1;
                  ui->lbConnectStatus->setText("未连接");
                  conStatus=false;
                  cntM=0;
                  cntS=0;
                 }
         }

        lastTriger=val;


    }

    else//链接不成功，如果自动重联，则每20秒重新链接一次
    {
        reconCnt++;
        if(ui->chbAutoLogOn->checkState()==Qt::Checked)//自动重连
        {
            if(reconCnt>=40)//重新链接
            {
                plcConn->myPLCip=ui->lePLCip->text();
                plcConn->myPLCport=ui->lePLCport->text().toInt();
                plcConn->start();
                reconCnt=0;
            }
        }
    }


    if(privCnt<600)
    {
        if(ui->tabWidget->currentIndex()!=6)
            privCnt++;
        if(privCnt==600)
        {
            ui->tab2->setEnabled(false);
        }
    }

    //sc
    if(last_cnt_sc!=sc_cnt)
    {
        last_cnt_sc=sc_cnt;
        cntSC=0;
    }
    else
    {
        cntSC++;
        if(cntSC>=5)
        {
            cntSC=0;
            last_cnt_sc=0;sc_cnt=0;
            g_ScanCode.clear();
        }
    }

    //flag
    bool scflag=false;
    if(mc_read_bool(socketfd, m_IniFile->value("ScFlagAddr").toString().toLocal8Bit().data(), &scflag))
    {
        if(scflag)
        {
            flagcnt++;
            if(flagcnt >=4)
            {
              flagcnt=0;
              mc_write_bool(socketfd, m_IniFile->value("ScFlagAddr").toString().toLocal8Bit().data(), false);

            }
          ui->lbWrFlag->setText("1");
        }
        else
        {
           flagcnt=0;
           ui->lbWrFlag->setText("0");
        }
    }

    if(mc_read_bool(socketfd, m_IniFile->value("ScFlagAddr2").toString().toLocal8Bit().data(), &scflag))
    {
        if(scflag)
        {
            flagcnt2++;
            if(flagcnt2 >=4)
            {
              flagcnt2=0;
              mc_write_bool(socketfd, m_IniFile->value("ScFlagAddr2").toString().toLocal8Bit().data(), false);

            }
          ui->lbWrFlag2->setText("1");
        }
        else
        {
           flagcnt=0;
           ui->lbWrFlag2->setText("0");
        }
    }

    char* scancode=NULL;
    if( mc_read_string(socketfd,  m_IniFile->value("ScWrteAddr").toString().toLocal8Bit().data(), m_IniFile->value("ScMaxLength").toInt(), &scancode))
        {//读取数据成功
            QString scString = QString::fromLocal8Bit(scancode,m_IniFile->value("ScMaxLength").toInt());
            ui->lbWrAddr->setText(scString);
        }
    else
        ui->lbWrAddr->setText("未知");
    if( mc_read_string(socketfd,  m_IniFile->value("ScWrteAddr2").toString().toLocal8Bit().data(), m_IniFile->value("ScMaxLength").toInt(), &scancode))
        {//读取数据成功
            QString scString = QString::fromLocal8Bit(scancode,m_IniFile->value("ScMaxLength").toInt());
            ui->lbWrAddr2->setText(scString);
        }
    else
        ui->lbWrAddr2->setText("未知");

   if(mc_read_bool(socketfd, m_IniFile->value("ManualInputAddr").toString().toLocal8Bit().data(), &scflag))
   {
       if(scflag){
           ui->lbManualTriger->setStyleSheet("QLabel{background-color:rgb(0,255,0);}");
           ui->lbManualTriger->setText("1");
           trigercnt ++;
           if(trigercnt >=4)
           {
             trigercnt=0;
             mc_write_bool(socketfd, m_IniFile->value("ManualInputAddr").toString().toLocal8Bit().data(), false);

           }

       }
       else{
           ui->lbManualTriger->setStyleSheet("QLabel{background-color: rgb(122, 122, 122);}");
           ui->lbManualTriger->setText("0");
       }

   }

}





#include "logindialog.h"

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
     Dialog logindlg;

    if ((index==1) && (ui->tab2->isEnabled()==false))
    {
         if (logindlg.exec() == QDialog::Accepted)
         {
             ui->tab2->setEnabled(true);
             privCnt=0;
         }
         else{
             ui->tab2->setEnabled(false);
         }
    }
}

#include "scancodedialog.h"

void MainWindow::show_scancode(QString str)
{
     QMessageBox * msgBox = new QMessageBox;

     ScDialog scdlg;
     scdlg.set_lab(str);
     scdlg.clear_text();
     scdlg.set_text(str);

showAgain:
     scdlg.open();
    if (scdlg.exec() == QDialog::Accepted)
     {
        QString ScanCode;
        ScanCode=scdlg.get_text();
        ScanCode=ScanCode.simplified();
        qDebug()<<ScanCode;

        if(scdlg.input_CH1==true and scdlg.input_CH2==false)
      {

        scdlg.close();

        mc_write_bool(socketfd, m_IniFile->value("ManualInputAddr").toString().toLocal8Bit().data(), true);

        if(ScanCode.length()>m_IniFile->value("ScMaxLength").toInt())
        {
         //   QMessageBox * msgBox = new QMessageBox;
               msgBox->setWindowTitle("告警");
               msgBox->setText("人检1扫码长度大于最大设置长度");
               msgBox->show();
        }
        else
        {
            const QByteArray local8Bit = ScanCode.toLocal8Bit();
            const char* charArray = local8Bit.data();
            if(mc_write_string(socketfd, m_IniFile->value("ScWrteAddr").toString().toLocal8Bit().data(), ScanCode.length(), charArray))
            {
           //     QMessageBox * msgBox = new QMessageBox;
                   msgBox->setWindowTitle("人检1投入扫码");
                   msgBox->setText("人检1投入扫码值成功，扫码长度为"+QString::number(ScanCode.length()));
                   msgBox->show();
            }
            else
            {
             //   QMessageBox * msgBox = new QMessageBox;
                   msgBox->setWindowTitle("人检1投入扫码");
                   msgBox->setText("人检1投入扫码值失败，请检测与PLC之间的通信。");
                   msgBox->show();
            }
            mc_write_bool(socketfd, m_IniFile->value("ScFlagAddr").toString().toLocal8Bit().data(), true);
        }
       }
      else  if(scdlg.input_CH1==false and scdlg.input_CH2==true)
      {

        scdlg.close();
        if(ScanCode.length()>m_IniFile->value("ScMaxLength").toInt())
        {
           // QMessageBox * msgBox = new QMessageBox;
               msgBox->setWindowTitle("告警");
               msgBox->setText("人检2扫码长度大于最大设置长度");
               msgBox->show();
        }
        else
        {
            const QByteArray local8Bit = ScanCode.toLocal8Bit();
            const char* charArray = local8Bit.data();
            if(mc_write_string(socketfd, m_IniFile->value("ScWrteAddr2").toString().toLocal8Bit().data(), ScanCode.length(), charArray))
            {
              //  QMessageBox * msgBox = new QMessageBox;
                   msgBox->setWindowTitle("人检2投入扫码");
                   msgBox->setText("人检2投入扫码值成功，扫码长度为"+QString::number(ScanCode.length()));
                   msgBox->show();
            }
            else
            {
              //  QMessageBox * msgBox = new QMessageBox;
                   msgBox->setWindowTitle("人检2投入扫码");
                   msgBox->setText("人检2投入扫码值失败，请检测与PLC之间的通信。");
                   msgBox->show();
            }
            mc_write_bool(socketfd, m_IniFile->value("ScFlagAddr2").toString().toLocal8Bit().data(), true);
        }
       }
       else if(scdlg.input_CH1==false and scdlg.input_CH2==false)
        {
            scdlg.close();
           // QMessageBox * msgBox = new QMessageBox;
               msgBox->setWindowTitle("写入扫码");
               msgBox->setText("未选择工位，请选择其中一个“人检投入”，工位1或工位2。");
               msgBox->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
               msgBox->show();
            goto showAgain;
        }
       else
        {
            scdlg.close();
           // QMessageBox * msgBox = new QMessageBox;
               msgBox->setWindowTitle("写入扫码");
               msgBox->setText("选择2个工位，请选择其中一个“人检投入”，工位1或工位2。");
               msgBox->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
               msgBox->show();
           goto showAgain;

        }


    }
}



void MainWindow::on_btWriteID_clicked()
{
    QString ScanCode;
    ScanCode=ui->teManID->toPlainText();
    qDebug()<<ScanCode;
    ScanCode=ScanCode.simplified();

    if(ScanCode.length()>m_IniFile->value("ScMaxLength").toInt())
    {
        QMessageBox * msgBox = new QMessageBox;
           msgBox->setWindowTitle("告警");
           msgBox->setText("人检1扫码长度大于最大设置长度");
           msgBox->show();
    }
    else
    {
        const QByteArray local8Bit = ScanCode.toLocal8Bit();
        const char* charArray = local8Bit.data();
        if(mc_write_string(socketfd, m_IniFile->value("ScWrteAddr").toString().toLocal8Bit().data(), ScanCode.length(), charArray))
        {
            QMessageBox * msgBox = new QMessageBox;
               msgBox->setWindowTitle("人检1投入扫码");
               msgBox->setText("人检1投入扫码值成功，扫码长度为"+QString::number(ScanCode.length()));
               msgBox->show();
        }
        else
        {
            QMessageBox * msgBox = new QMessageBox;
               msgBox->setWindowTitle("人检1投入扫码");
               msgBox->setText("人检1投入扫码值失败，请检测与PLC之间的通信。");
               msgBox->show();
        }
        mc_write_bool(socketfd, m_IniFile->value("ScFlagAddr").toString().toLocal8Bit().data(), true);
    }

}



void MainWindow::on_btWriteID_2_clicked()
{
    QString ScanCode;
    ScanCode=ui->teManID->toPlainText();
    qDebug()<<ScanCode;
    ScanCode=ScanCode.simplified();

    if(ScanCode.length()>m_IniFile->value("ScMaxLength").toInt())
    {
        QMessageBox * msgBox = new QMessageBox;
           msgBox->setWindowTitle("告警");
           msgBox->setText("人检2扫码长度大于最大设置长度");
           msgBox->show();
    }
    else
    {
        const QByteArray local8Bit = ScanCode.toLocal8Bit();
        const char* charArray = local8Bit.data();
        if(mc_write_string(socketfd, m_IniFile->value("ScWrteAddr2").toString().toLocal8Bit().data(), ScanCode.length(), charArray))
        {
            QMessageBox * msgBox = new QMessageBox;
               msgBox->setWindowTitle("人检2投入扫码");
               msgBox->setText("人检2投入扫码值成功，扫码长度为"+QString::number(ScanCode.length()));
               msgBox->show();
        }
        else
        {
            QMessageBox * msgBox = new QMessageBox;
               msgBox->setWindowTitle("人检2投入扫码");
               msgBox->setText("人检2投入扫码值失败，请检测与PLC之间的通信。");
               msgBox->show();
        }
        mc_write_bool(socketfd, m_IniFile->value("ScFlagAddr2").toString().toLocal8Bit().data(), true);
    }

}



void MainWindow::on_btManualTriger_clicked()
{
#if 0
    QMessageBox messageBox;
    QFont defaultFont = QApplication::font(); // 获取默认字体
    defaultFont.setPixelSize(16);
    messageBox.setFont(defaultFont);
    messageBox.setWindowFlags( Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
   // messageBox.setFixedSize(800,480);//设置MessageBox的大小
    //messageBox.setStyleSheet("background-image:url(:/new/two/two_1.jpeg)");//显示图片
    messageBox.setWindowTitle("人检投入选择");
    messageBox.setText("码值:"+ui->teManID->toPlainText().simplified());
    messageBox.setInformativeText("请选择其中一个工位进行投片！");
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox.setDefaultButton(QMessageBox::No);
    messageBox.setButtonText(QMessageBox::Yes, "人检1投片");
    messageBox.setButtonText(QMessageBox::No, "人检2投片");
        int result = messageBox.exec();

        if (result == QMessageBox::Yes) {
            on_btWriteID_clicked();

        } else  if(result == QMessageBox::No) {

            on_btWriteID_2_clicked();
        }
#endif
    if(mc_write_bool(socketfd, m_IniFile->value("ManualInputAddr").toString().toLocal8Bit().data(), true))
    {

    }
    else
    {
           QMessageBox * msgBox = new QMessageBox;
           msgBox->setWindowTitle("人检投入触发");
           msgBox->setText("人检投入触发失败，请检测与PLC之间的通信。");
           msgBox->show();
    }

}


