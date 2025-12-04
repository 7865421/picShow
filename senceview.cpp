#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include<QMessageBox>

QList<QGraphicsItem*> MainWindow::cloneItems(const QList<QGraphicsItem*>& items) {
    QHash<QGraphicsItem*, QGraphicsItem*> copyMap;
    foreach (QGraphicsItem* item, items) {
        if (item->type() == DiagramItem::Type) {
            copyMap[item] = qgraphicsitem_cast<DiagramItem*>(item)->clone();
        } else if (item->type() == DiagramTextItem::Type) {
            copyMap[item] = qgraphicsitem_cast<DiagramTextItem*>(item)->clone();
        }
    }

    return copyMap.values();
}

void MainWindow::copyItem(DiagramScene *scene) {
    foreach(QGraphicsItem* p, pasteBoard) {
        delete p;
    }
    pasteBoard = cloneItems(scene->selectedItems());
    qDebug() << pasteBoard.size();
}

void MainWindow::pasteItem(DiagramScene *scene) {
    QList<QGraphicsItem*> pasteBoardCopy(cloneItems(pasteBoard));
    foreach(QGraphicsItem* p, scene->items()) p->setSelected(false);

    foreach(QGraphicsItem* item, pasteBoard) {

        scene->addItem(item);
        item->setSelected(true);
    }
    pasteBoard.swap(pasteBoardCopy);
    if (!scene->selectedItems().empty())
        undoStack.backup(cloneItems(scene->items()));
}

void MainWindow::cutItem(DiagramScene *scene) {
    copyItem(scene);
    deleteItem(scene);
}

void MainWindow::deleteItem(DiagramScene *scene) {
    bool needsBackup = !scene->selectedItems().empty();
    scene->deleteItems(scene->selectedItems());
    if (needsBackup)
        undoStack.backup(cloneItems(scene->items()));
}

void MainWindow::undo(DiagramScene *scene) {
    if (undoStack.isEmpty()) return;
    // sweep away all items
    scene->deleteItems(scene->items());
    QList<QGraphicsItem*> undoneItems = cloneItems(undoStack.undo());
    foreach(QGraphicsItem* item, undoneItems) {
        qDebug() << item << "--------- add item";
        scene->addItem(item);
    }


}

void MainWindow::redo(DiagramScene *scene) {
    if (undoStack.isFull()) return;
    scene->deleteItems(scene->items());
    QList<QGraphicsItem*> redoneItems = cloneItems(undoStack.redo());
    foreach(QGraphicsItem* item, redoneItems) {
        scene->addItem(item);
    }


}

void MainWindow::groupItems(DiagramScene *scene) {
    QGraphicsItemGroup* group = scene->createItemGroup(scene->selectedItems());
    group->setFlag(QGraphicsItem::ItemIsMovable, true);
    group->setFlag(QGraphicsItem::ItemIsSelectable, true);
    group->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    scene->addItem(group);
    backupUndostack(scene);
}

void MainWindow::ungroupItems(DiagramScene *scene) {
    foreach(QGraphicsItem* p, scene->selectedItems()) {
        if (p->type() == QGraphicsItemGroup::Type) {
            scene->destroyItemGroup(qgraphicsitem_cast<QGraphicsItemGroup*>(p));
        }
    }
    backupUndostack(scene);
}

void MainWindow::bringToFront(DiagramScene *scene)
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() >= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
    backupUndostack(scene);
}
//! [5]

//! [6]
void MainWindow::sendToBack(DiagramScene *scene)
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() <= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);
    backupUndostack(scene);
}
//! [6]

//! [7]
void MainWindow::itemInserted(DiagramScene *scene,DiagramItem *item)
{
//    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    scene->setMode(DiagramScene::MoveItem);
    scene->update();
    backupUndostack(scene);
}
//! [7]

//! [8]
void MainWindow::textInserted(DiagramScene *scene,QGraphicsTextItem *)
{
 //   buttonGroup->button(InsertTextButton)->setChecked(false);
  //  scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}

void MainWindow::backupUndostack(DiagramScene *scene) {
    undoStack.backup(cloneItems(scene->items()));
}
//! [8]

//! [9]
void MainWindow::currentFontChanged(DiagramScene *scene,const QFont &)
{
    handleFontChange(scene);
}
//! [9]

//! [10]
void MainWindow::fontSizeChanged(DiagramScene *scene,const QString &)
{
    handleFontChange(scene);
}
//! [10]

//! [11]
void MainWindow::sceneScaleChanged(const QString &scale)
{
    double newScale = scale.toDouble() / 100.0;
    QMatrix oldMatrix = ui->graphicsView1->matrix();

    //if(ui->tabWidget->currentIndex()==0){
    if(ui->graphicsView1->underMouse()){
    ui->graphicsView1->resetMatrix();
    ui->graphicsView1->translate(oldMatrix.dx(), oldMatrix.dy());
    ui->graphicsView1->scale(newScale, newScale);
   // qDebug()<<"emit scale change";
    }
  //  else if(ui->tabWidget->currentIndex()==1){
    else if(ui->graphicsView2->underMouse()){
        ui->graphicsView2->resetMatrix();
        ui->graphicsView2->translate(oldMatrix.dx(), oldMatrix.dy());
        ui->graphicsView2->scale(newScale, newScale);
       // qDebug()<<"emit scale change";
    }
   // else if(ui->tabWidget->currentIndex()==2){
    else if(ui->graphicsView3->underMouse()){
        ui->graphicsView3->resetMatrix();
        ui->graphicsView3->translate(oldMatrix.dx(), oldMatrix.dy());
        ui->graphicsView3->scale(newScale, newScale);
       // qDebug()<<"emit scale change";
    }
  //  else if(ui->tabWidget->currentIndex()==3){
    else if(ui->graphicsView4->underMouse()){
        ui->graphicsView4->resetMatrix();
        ui->graphicsView4->translate(oldMatrix.dx(), oldMatrix.dy());
        ui->graphicsView4->scale(newScale, newScale);
       // qDebug()<<"emit scale change";
    }
    //else if(ui->tabWidget->currentIndex()==4){
    else if(ui->graphicsView5->underMouse()){
        ui->graphicsView5->resetMatrix();
        ui->graphicsView5->translate(oldMatrix.dx(), oldMatrix.dy());
        ui->graphicsView5->scale(newScale, newScale);
       // qDebug()<<"emit scale change";
    }
    //else {
    else if(ui->graphicsView6->underMouse()){
        ui->graphicsView6->resetMatrix();
        ui->graphicsView6->translate(oldMatrix.dx(), oldMatrix.dy());
        ui->graphicsView6->scale(newScale, newScale);
       // qDebug()<<"emit scale change";
    }
    else
    {
        //nothing here
    }
}

void MainWindow::scene6ScaleZooming(int delta) {
    static int scale=50;
    int changingPercent = delta > 0 ? 10 : -10;
    int newScale = scale + changingPercent;
   // qDebug()<<"do zooming scale change"<< newScale;
    if (newScale > 0 && newScale <= 400) {
        scale=newScale;
    }
  //  qDebug()<<"do zooming scale change";

    sceneScaleChanged(QString::number(newScale));
}

void MainWindow::scene1ScaleZooming(int delta) {
    static int scale=50;
    int changingPercent = delta > 0 ? 10 : -10;
    int newScale = scale + changingPercent;
   // qDebug()<<"do zooming scale change"<< newScale;
    if (newScale > 0 && newScale <= 400) {
        scale=newScale;
    }
  //  qDebug()<<"do zooming scale change";

    sceneScaleChanged(QString::number(newScale));
}
void MainWindow::scene2ScaleZooming(int delta) {
    static int scale=50;
    int changingPercent = delta > 0 ? 10 : -10;
    int newScale = scale + changingPercent;
   // qDebug()<<"do zooming scale change"<< newScale;
    if (newScale > 0 && newScale <= 400) {
        scale=newScale;
    }
  //  qDebug()<<"do zooming scale change";

    sceneScaleChanged(QString::number(newScale));
}
void MainWindow::scene3ScaleZooming(int delta) {
    static int scale=50;
    int changingPercent = delta > 0 ? 10 : -10;
    int newScale = scale + changingPercent;
   // qDebug()<<"do zooming scale change"<< newScale;
    if (newScale > 0 && newScale <= 400) {
        scale=newScale;
    }
  //  qDebug()<<"do zooming scale change";

    sceneScaleChanged(QString::number(newScale));
}
void MainWindow::scene4ScaleZooming(int delta) {
    static int scale=50;
    int changingPercent = delta > 0 ? 10 : -10;
    int newScale = scale + changingPercent;
   // qDebug()<<"do zooming scale change"<< newScale;
    if (newScale > 0 && newScale <= 400) {
        scale=newScale;
    }
  //  qDebug()<<"do zooming scale change";

    sceneScaleChanged(QString::number(newScale));
}
void MainWindow::scene5ScaleZooming(int delta) {
    static int scale=50;
    int changingPercent = delta > 0 ? 10 : -10;
    int newScale = scale + changingPercent;
   // qDebug()<<"do zooming scale change"<< newScale;
    if (newScale > 0 && newScale <= 400) {
        scale=newScale;
    }
  //  qDebug()<<"do zooming scale change";

    sceneScaleChanged(QString::number(newScale));
}



//! [11]

//! [12]
void MainWindow::textColorChanged(DiagramScene *scene)
{
    /*
    textAction = qobject_cast<QAction *>(sender());
    fontColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/textpointer.png",
                                     qvariant_cast<QColor>(textAction->data())));
    textButtonTriggered();
    undoStack.backup(cloneItems(scene->items()));
    */
}
//! [12]

//! [13]
void MainWindow::itemColorChanged(DiagramScene *scene)
{
    /*
    fillAction = qobject_cast<QAction *>(sender());
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/floodfill.png",
                                     qvariant_cast<QColor>(fillAction->data())));
    fillButtonTriggered();
    undoStack.backup(cloneItems(scene->items()));
    */
}
//! [13]

//! [14]
void MainWindow::lineColorChanged(DiagramScene *scene)
{
    /*
    lineAction = qobject_cast<QAction *>(sender());
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/linecolor.png",
                                     qvariant_cast<QColor>(lineAction->data())));
    lineButtonTriggered();
    */
}
//! [14]

//! [15]
void MainWindow::textButtonTriggered(DiagramScene *scene)
{
    scene->setTextColor(qvariant_cast<QColor>(textAction->data()));
}
//! [15]

//! [16]
void MainWindow::fillButtonTriggered(DiagramScene *scene)
{
    scene->setItemColor(qvariant_cast<QColor>(fillAction->data()));
}
//! [16]

//! [17]
void MainWindow::lineButtonTriggered(DiagramScene *scene)
{
    scene->setLineColor(qvariant_cast<QColor>(lineAction->data()));
}
//! [17]

//! [18]
void MainWindow::handleFontChange(DiagramScene *scene)
{
    /*
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());

    scene->setFont(font);
    */
}
//! [18]

//! [19]
void MainWindow::itemSelected(DiagramScene *scene,QGraphicsItem *item)
{
    DiagramTextItem *textItem =
    qgraphicsitem_cast<DiagramTextItem *>(item);

    QFont font = textItem->font();
    //fontCombo->setCurrentFont(font);
 //   fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    boldAction->setChecked(font.weight() == QFont::Bold);
    italicAction->setChecked(font.italic());
    underlineAction->setChecked(font.underline());
}
//! [19]

//! [20]
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Diagram Scene"),
                       tr("A drawing tool based on Qt Example."));
}


void MainWindow::createActions()
{
    toFrontAction = new QAction(QIcon(":/images/bringtofront.png"),
                                tr("上移到顶层"), this);
    toFrontAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
    toFrontAction->setStatusTip(tr("把部件移动到顶层"));
    //connect(toFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront(DiagramScene *scene)));

    sendBackAction = new QAction(QIcon(":/images/sendtoback.png"), tr("下移到底层"), this);
   // sendBackAction->setShortcut(tr("Ctrl+T"));
    sendBackAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
    sendBackAction->setStatusTip(tr("下移到底层k"));
    //connect(sendBackAction, SIGNAL(triggered()), this, SLOT(sendToBack(DiagramScene *scene)));

    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("删除"), this);
   // deleteAction->setShortcut(tr("Delete"));
    deleteAction->setShortcut(QKeySequence(Qt::Key_Delete));
    deleteAction->setStatusTip(tr("删除部件"));
    //connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem(DiagramScene *scene)));

    exitAction = new QAction(tr("退出"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("退出程序"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    boldAction = new QAction(tr("加粗"), this);
    boldAction->setCheckable(true);
    QPixmap pixmap(":/images/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(tr("Ctrl+B"));
    //connect(boldAction, SIGNAL(triggered()), this, SLOT(handleFontChange(DiagramScene *scene)));

    italicAction = new QAction(QIcon(":/images/italic.png"), tr("斜体"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    //connect(italicAction, SIGNAL(triggered()), this, SLOT(handleFontChange(DiagramScene *scene)));

    underlineAction = new QAction(QIcon(":/images/underline.png"), tr("下划线"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    //connect(underlineAction, SIGNAL(triggered()), this, SLOT(handleFontChange(DiagramScene *scene)));

    aboutAction = new QAction(tr("关于"), this);
    aboutAction->setShortcut(tr("F1"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    copyAction = new QAction(QIcon(":/images/copy.png"), tr("复制"), this);
    copyAction->setShortcut(tr("Ctrl+C"));
    copyAction->setStatusTip(tr("复制选中部件"));
    //connect(copyAction, SIGNAL(triggered()), this, SLOT(copyItem(DiagramScene *scene)));

    pasteAction = new QAction(QIcon(":/images/paste.png"), tr("粘贴"), this);
    pasteAction->setShortcut(tr("Ctrl+V"));
    pasteAction->setStatusTip(tr("从粘贴板中粘贴部件"));
    //connect(pasteAction, SIGNAL(triggered()), this, SLOT(pasteItem(DiagramScene *scene)));

    cutAction = new QAction(QIcon(":/images/cut.png"), tr("剪切"), this);
    cutAction->setShortcut(tr("Ctrl+X"));
    cutAction->setStatusTip(tr("剪切部件"));
   // connect(cutAction, SIGNAL(triggered()), this, SLOT(cutItem(DiagramScene *scene)));

    undoAction = new QAction(QIcon(":images/undo.png"), tr("撤销"), this);
    undoAction->setShortcut(tr("Ctrl+Z"));
    undoAction->setStatusTip(tr("撤销操作"));
    //connect(undoAction, SIGNAL(triggered()), this, SLOT(undo(DiagramScene *scene)));

    redoAction = new QAction(QIcon(":images/redo.png"), tr("恢复"), this);
    redoAction->setShortcut(tr("Ctrl+Shift+Z"));
    redoAction->setStatusTip(tr("恢复上次操作"));
    //connect(redoAction, SIGNAL(triggered()), this, SLOT(redo(DiagramScene *scene)));

    groupAction = new QAction(QIcon(":images/group.png"), tr("组合"), this);
    groupAction->setShortcut(tr("Ctrl+G"));
    groupAction->setStatusTip(tr("组合选中部件 "));
  //  connect(groupAction, SIGNAL(triggered()), this, SLOT(groupItems(DiagramScene *scene)));

    ungroupAction = new QAction(QIcon(":images/ungroup.png"), tr("拆分"), this);
    ungroupAction->setShortcut(tr("Ctrl+U"));
    ungroupAction->setStatusTip(tr("拆分组合部件"));
    //connect(ungroupAction, SIGNAL(triggered()), this, SLOT(ungroupItems(DiagramScene *scene)));
}

void MainWindow::createMenus()
{

   // itemMenu = ui->menuBar->addMenu(tr("&Item"));
    itemMenu = new QMenu(this);
    itemMenu->addAction(copyAction);
    itemMenu->addAction(cutAction);
    itemMenu->addAction(pasteAction);
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(undoAction);
    itemMenu->addAction(redoAction);
    itemMenu->addSeparator();
    itemMenu->addAction(groupAction);
    itemMenu->addAction(ungroupAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);

   // aboutMenu = ui->menuBar->addMenu(tr("&Help"));
    aboutMenu =new QMenu(this);
    aboutMenu->addAction(aboutAction);
}

void MainWindow::config_init()
{
    m_IniFile = new QSettings(QCoreApplication::applicationDirPath()+"/etc/config.ini", QSettings::IniFormat);
    m_IniFile->setIniCodec("utf-8");

//自动获取设备类型
    if(m_IniFile->contains("AutoType")==false)
        m_IniFile->setValue("AutoType","1");

    if(m_IniFile->value("AutoType").toString()=="0")
        ui->chbAutoType->setCheckState(Qt::Unchecked);
    else
        ui->chbAutoType->setCheckState(Qt::Checked);

//参数配置自动登录
    if(m_IniFile->contains("AutoLogOn")==false)
        m_IniFile->setValue("AutoLogOn","1");

    if(m_IniFile->value("AutoLogOn").toString()=="0")
        ui->chbAutoLogOn->setCheckState(Qt::Unchecked);
    else
        ui->chbAutoLogOn->setCheckState(Qt::Checked);

//参数配置添加日期
    if(m_IniFile->contains("AddDate")==false)
        m_IniFile->setValue("AddDate","1");

    if(m_IniFile->value("AddDate").toString()=="0"){
        ui->chbAddDate->setCheckState(Qt::Unchecked);

    }
    else
        ui->chbAddDate->setCheckState(Qt::Checked);

//参数配置根据ID获取
    if(m_IniFile->contains("OpenByID")==false)
         m_IniFile->setValue("OpenByID","1");

    if(m_IniFile->value("OpenByID").toString()=="0")
        ui->chbAutoRead->setCheckState(Qt::Unchecked);
    else
        ui->chbAutoRead->setCheckState(Qt::Checked);

//参数配置PLCIP
    if(m_IniFile->contains("PLCIP")==false)
        m_IniFile->setValue("PLCIP","192.168.19.204");
    ui->lePLCip->setText(m_IniFile->value("PLCIP").toString());
//参数配置PLCPORT
    if(m_IniFile->contains("PLCPORT")==false)
        m_IniFile->setValue("PLCPORT","5002");
    ui->lePLCport->setText(m_IniFile->value("PLCPORT").toString());
//参数配置 触发数据地址
    if(m_IniFile->contains("TrigerAddr")==false)
        m_IniFile->setValue("TrigerAddr","M200");
    ui->leTrigerAddr->setText(m_IniFile->value("TrigerAddr").toString());
//参数配置 ID地址
     if(m_IniFile->contains("IDAddr")==false)
          m_IniFile->setValue("IDAddr","D2000");
     ui->leIDaddr->setText(m_IniFile->value("IDAddr").toString());
//ID长度
     if(m_IniFile->contains("IDLength")==false)
          m_IniFile->setValue("IDLength","20");
     ui->leIDmax->setText(m_IniFile->value("IDLength").toString());
//第一图片基础地址
      if(m_IniFile->contains("PicBaseDir1")==false)
          m_IniFile->setValue("PicBaseDir1","//10.10.56.88/Pic1");
      ui->lePicBaseDir1->setText(m_IniFile->value("PicBaseDir1").toString());
//第二图片基础地址
      if(m_IniFile->contains("PicBaseDir2")==false)
          m_IniFile->setValue("PicBaseDir2","//10.10.56.88/Pic2");
      ui->lePicBaseDir2->setText(m_IniFile->value("PicBaseDir2").toString());
//第三图片基础地址
      if(m_IniFile->contains("PicBaseDir3")==false)
          m_IniFile->setValue("PicBaseDir3","//10.10.56.88/Pic3");
       ui->lePicBaseDir3->setText(m_IniFile->value("PicBaseDir3").toString());
//第一台设备类型
       if(m_IniFile->contains("DevType1")==false)
           m_IniFile->setValue("DevType1","13");
       ui->leDevType1->setText(m_IniFile->value("DevType1").toString());
//第二台设备类型
       if(m_IniFile->contains("DevType2")==false)
           m_IniFile->setValue("DevType2","13");
        ui->leDevType2->setText(m_IniFile->value("DevType2").toString());
//第三台设备类型
       if(m_IniFile->contains("DevType3")==false)
           m_IniFile->setValue("DevType3","13");
       ui->leDevType3->setText(m_IniFile->value("DevType3").toString());

       if(m_IniFile->contains("DirBack1")==false)
           m_IniFile->setValue("DirBack1","OUT-");
       ui->leDirBack1->setText(m_IniFile->value("DirBack1").toString());

       if(m_IniFile->contains("DirBack2")==false)
           m_IniFile->setValue("DirBack2","IN-");
       ui->leDirBack2->setText(m_IniFile->value("DirBack2").toString());

       if(m_IniFile->contains("DirBack3")==false)
           m_IniFile->setValue("DirBack3","IN-");
       ui->leDirBack3->setText(m_IniFile->value("DirBack3").toString());

       if(m_IniFile->contains("TypeAddr1")==false)
           m_IniFile->setValue("TypeAddr1","D16010");
       ui->leDevTypeAddr1->setText(m_IniFile->value("TypeAddr1").toString());

       if(m_IniFile->contains("TypeAddr2")==false)
           m_IniFile->setValue("TypeAddr2","D16014");
       ui->leDevTypeAddr2->setText(m_IniFile->value("TypeAddr2").toString());

       if(m_IniFile->contains("TypeAddr3")==false)
           m_IniFile->setValue("TypeAddr3","D16018");
       ui->leDevTypeAddr3->setText(m_IniFile->value("TypeAddr3").toString());

       if(m_IniFile->contains("TypeLength")==false)
           m_IniFile->setValue("TypeLength","2");
       ui->leTypeLenth->setText(m_IniFile->value("TypeLength").toString());



}
