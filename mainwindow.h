#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include<QTimer>
#include<QDateTime>

#include "diagramscene.h"
#include "undosystem.h"
#include "connection.h"
//#include "globalkeyevent.h"
#include "scancodedialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
     bool eventFilter(QObject *obj, QEvent *e);
signals:
void get_scancode(QString str);
private slots:

    void plcConnectDone(int fd);
    void plcRead();
   // void backgroundButtonGroupClicked(QAbstractButton *button);
   // void buttonGroupClicked(int id);
    void copyItem(DiagramScene *scene);
    void pasteItem(DiagramScene *scene);
    void cutItem(DiagramScene *scene);
    void deleteItem(DiagramScene *scene);
    void undo(DiagramScene *scene);
    void redo(DiagramScene *scene);
    void groupItems(DiagramScene *scene);
    void ungroupItems(DiagramScene *scene);
    void bringToFront(DiagramScene *scene);
    void sendToBack(DiagramScene *scene);
    void itemInserted(DiagramScene *scene,DiagramItem *item);
    void textInserted(DiagramScene *scene,QGraphicsTextItem *item);
    void backupUndostack(DiagramScene *scene);
    void currentFontChanged(DiagramScene *scene,const QFont &font);
    void fontSizeChanged(DiagramScene *scene,const QString &size);
    void sceneScaleChanged(const QString &scale);
    void scene1ScaleZooming(int delta);
    void scene2ScaleZooming(int delta);
    void scene3ScaleZooming(int delta);
    void scene4ScaleZooming(int delta);
    void scene5ScaleZooming(int delta);
    void scene6ScaleZooming(int delta);

    void textColorChanged(DiagramScene *scene);
    void itemColorChanged(DiagramScene *scene);
    void lineColorChanged(DiagramScene *scene);
    void textButtonTriggered(DiagramScene *scene);
    void fillButtonTriggered(DiagramScene *scene);
    void lineButtonTriggered(DiagramScene *scene);
    void handleFontChange(DiagramScene *scene);
    void itemSelected(DiagramScene *scene,QGraphicsItem *item);
    void about();
    void on_keyEvent(QKeyEvent* event);
    void show_scancode(QString str);
    //void Route();

    void on_btOpenPic1_clicked();

    void on_lePLCip_textChanged(const QString &arg1);

    void on_lePLCport_textChanged(const QString &arg1);

    void on_leTrigerAddr_textChanged(const QString &arg1);

    void on_leIDaddr_textChanged(const QString &arg1);

    void on_leIDmax_textChanged(const QString &arg1);



    void on_lePicBaseDir1_textChanged(const QString &arg1);

    void on_lePicBaseDir2_textChanged(const QString &arg1);

    void on_lePicBaseDir3_textChanged(const QString &arg1);



    void on_chbAddDate_stateChanged(int arg1);

    void on_chbAutoRead_stateChanged(int arg1);

    void on_chbAutoLogOn_stateChanged(int arg1);

    void on_btBrowseBaseDir1_clicked();

    void on_btBrowseBaseDir2_clicked();

    void on_btBrowseBaseDir3_clicked();



    void on_btOpenPic2_clicked();

    void on_btOpenPic3_clicked();

    void on_btOpenPic4_clicked();

    void on_btOpenPic5_clicked();

    void on_btOpenPic6_clicked();

    void on_btConnect_clicked();

    void on_leDevType1_textChanged(const QString &arg1);

    void on_leDevType2_textChanged(const QString &arg1);

    void on_leDevType3_textChanged(const QString &arg1);

    void on_leDirBack1_textChanged(const QString &arg1);

    void on_leDirBack2_textChanged(const QString &arg1);

    void on_leDirBack3_textChanged(const QString &arg1);

    void on_chbAutoType_stateChanged(int arg1);

    void on_leDevTypeAddr1_textChanged(const QString &arg1);

    void on_leDevTypeAddr2_textChanged(const QString &arg1);

    void on_leDevTypeAddr3_textChanged(const QString &arg1);

    void on_leTypeLenth_textChanged(const QString &arg1);

    void on_tabWidget_tabBarClicked(int index);

    void on_leScMax_textChanged(const QString &arg1);



    void on_leScWrAddr_textChanged(const QString &arg1);

    void on_leScFlagAddr_textChanged(const QString &arg1);

    void on_leTransAngle_textChanged(const QString &arg1);

    void on_btWriteID_clicked();

    void on_leScWrAddr2_textChanged(const QString &arg1);

    void on_btWriteID_2_clicked();

    void on_leScFlagAddr2_textChanged(const QString &arg1);

    void on_btManualTriger_clicked();

    void on_leInputTriger_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    void StartInit();
    void config_init();
public:

    Connection *plcConn;
    int socketfd=-1;
    bool conStatus=false;

    QSettings *m_IniFile;

    QTimer *PLCreadTimer;//PLC读定时器

    QString DateString="";
    QString IDString="";

    QDateTime datetime;

    int viewSelect=0;

    unsigned int privCnt=0;//权限定时计数器

    DiagramScene *scene1;//图像显示
    DiagramScene *scene2;//图像显示
    DiagramScene *scene3;//图像显示
    DiagramScene *scene4;//图像显示
    DiagramScene *scene5;//图像显示
    DiagramScene *scene6;//图像显示
   // QGraphicsView *view;

    QAction *exitAction;
    QAction *addAction;
    QAction *deleteAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *cutAction;
    QAction *undoAction;
    QAction *redoAction;

    QAction *toFrontAction;
    QAction *sendBackAction;
    QAction *groupAction;
    QAction *ungroupAction;

    QAction *aboutAction;

    QAction *boldAction;
    QAction *underlineAction;
    QAction *italicAction;
    QAction *textAction;
    QAction *fillAction;
    QAction *lineAction;

    QMenu *fileMenu;
    QMenu *itemMenu;
    QMenu *aboutMenu;

    QList<QGraphicsItem*> cloneItems(QList<QGraphicsItem*> const& items);
    QList<QGraphicsItem*> pasteBoard;
    UndoSystem undoStack;

    //ScDialog scdlg;

    int sc_cnt=0;//3秒内如果没有键盘按下，则键盘接收值请0
    QString g_ScanCode;
    void createActions();
    void createMenus();
};
#endif // MAINWINDOW_H
