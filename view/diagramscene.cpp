/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "diagramscene.h"


#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QMenu>
#include <QKeyEvent>
QPen const DiagramScene::penForLines = QPen(QBrush(QColor(Qt::black)), 2, Qt::PenStyle::DashLine);

DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    myItemType = DiagramItem::Step;
    line = nullptr;
    textItem = nullptr;
    myItemColor =  QColor(0,0,0,0);
    myTextColor = Qt::green;
    myLineColor = Qt::green;


    QDomProcessingInstruction instruction; //添加xml说明
    instruction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement root=doc.createElement("VsionTemple"); //创建根元素
    doc.appendChild(root); //添加根元素
}

DiagramScene::DiagramScene()
{

    myMode = MoveItem;
    myItemType = DiagramItem::Step;
    line = nullptr;
    textItem = nullptr;
    myItemColor =  QColor(0,0,0,0);
    myTextColor = Qt::green;
    myLineColor = Qt::green;

 //   myItemMenu= new QMenu(this);

    QDomProcessingInstruction instruction; //添加xml说明
    instruction=doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    QDomElement root=doc.createElement("VsionTemple"); //创建根元素
    doc.appendChild(root); //添加根元素
}


bool DiagramScene::domhasChild(QString tag)//查询是否有该节点
{
    QDomElement root = doc.documentElement();//读取根节点
    //qDebug()<<"寻找根节点"<<root.tagName();
    QDomNode rnode = root.firstChild();
   // qDebug()<<"第一个节点"<<rnode.toElement().tagName();
    while(!rnode.isNull())
     {
         QString tagName = rnode.toElement().tagName();  //节点元素名称
        //  qDebug()<<"遍历所有节点"<<tagName;
         if(tagName==tag)
         {
            return true;
         }
         rnode=rnode.nextSibling();
     }
    return false;
}
//有节点，返回1，无节点，新建节点，返回0；
bool DiagramScene::setNodeAttribute(QString tag,QString Attribute,QString value)//重新设置节点属性值//重新设置节点属性值
{
    QDomElement root = doc.documentElement();//读取根节点
   // qDebug()<<"寻找根节点"<<root.tagName();
    QDomNode rnode = root.firstChild();
   // qDebug()<<"第一个节点"<<rnode.toElement().tagName();
    while(!rnode.isNull())
     {
         QString tagName = rnode.toElement().tagName();  //节点元素名称
   //       qDebug()<<"遍历所有节点"<<tagName;
         if(tagName==tag)
         {
        //     qDebug()<<"拥有"<<tag<<"节点";
             rnode.toElement().setAttribute(Attribute,value);
          //   qDebug()<<doc.toString();
             return true;
         }
         rnode=rnode.nextSibling();
     }

    QDomElement Element=doc.createElement(tag); //一级子元素
    Element.setAttribute(Attribute,value);
    root.appendChild(Element);
    //qDebug()<<"无"<<tag<<"节点，创建节点"<<doc.toString();
    return false;

}
QString DiagramScene::getNodeAttribute(QString tag,QString Attribute)//获取节点属性值，无值返回空
{
    QDomElement root = doc.documentElement();//读取根节点

    QDomNode rnode = root.firstChild();//读取第一个子节点   QDomNode 节点
    while (!rnode.isNull())
    {
        QString tagName = rnode.toElement().tagName();  //节点元素名称
        if(tagName==tag)
        {
            QString attrValue;
            attrValue=rnode.toElement().attribute( Attribute ,QString(""));
            if(attrValue != "")
                return rnode.toElement().attribute( Attribute );

        }
        rnode = rnode.nextSibling();//读取下一个兄弟节点
    }

    return "";
}



void DiagramScene::setLineColor(const QColor &color)
{
    myLineColor = color;
    foreach (QGraphicsItem* p, selectedItems()) {

            DiagramItem* item = qgraphicsitem_cast<DiagramItem*>(p);
            item->setColor(myLineColor);
            update();


    }
}

void DiagramScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    foreach (QGraphicsItem* p, selectedItems()) {
        if (p->type() == DiagramTextItem::Type) {
            DiagramTextItem* item = qgraphicsitem_cast<DiagramTextItem*>(p);
            item->setDefaultTextColor(myTextColor);
        }
    }
}

void DiagramScene::setTextColor(DiagramTextItem  *item,QColor color)
{
    myTextColor = color;

     item->setDefaultTextColor(myTextColor);


}

//! [2]

//! [3]
void DiagramScene::setItemColor(const QColor &color)
{
    myItemColor = color;
    foreach (QGraphicsItem* p, selectedItems()) {
        if (p->type() == DiagramItem::Type) {
            DiagramItem* item = qgraphicsitem_cast<DiagramItem*>(p);
            item->setBrush(myItemColor);
        }
    }
}

void DiagramScene::setFont(const QFont &font)
{
    myFont = font;
    foreach (QGraphicsItem* p, selectedItems()) {
        if (p->type() == DiagramTextItem::Type) {
            DiagramTextItem* item = qgraphicsitem_cast<DiagramTextItem*>(p);
            item->setFont(myFont);
        }
    }
}

void DiagramScene::setFont(DiagramTextItem  *item,QFont font)
{
    myFont = font;

    item->setFont(myFont);
}

void DiagramScene::deleteItems(QList<QGraphicsItem*> const& items) {
    qDebug() << "delete items" << items;

    QList<QGraphicsItem*> diagramItems;
    foreach (QGraphicsItem *item, items) {
         diagramItems.append(item);
    }

    foreach (QGraphicsItem *item, diagramItems) {
        if (item->type() == DiagramItem::Type)

        removeItem(item);
        delete item;
    }
}
//! [4]

void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

void DiagramScene::setItemType(DiagramItem::DiagramType type)
{
    myItemType = type;
}

//! [5]
void DiagramScene::editorLostFocus(DiagramTextItem *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    } else {
        if (item->contentIsUpdated()) {
            qDebug() << "content update ---";
            emit textChanged();
        }
    }

}

QString DiagramScene::polygonToQstring(QPolygonF polygon)
{
    QList<QPointF> pointlist;
    QStringList strlist;
    QString str;
    pointlist =polygon.toList();

    for(int s=0;s<pointlist.size();s++)
    {

        strlist.append(QString().sprintf("%f %f", pointlist.at(s).x(),pointlist.at(s).y()));
    }
    str = strlist.join(",");
    return str;
}
QPolygonF DiagramScene::qstringToPolygon(QString str)
{
    QList<QPointF> pointlist;
    QStringList strlist,plist;
    QPointF point;
    QString pointstr,pointx,pointy;
    QPolygonF polygon;

    strlist =str.split(",");

    for(int s=0;s<strlist.size();s++)
    {
        pointstr=strlist.at(s);
        plist=pointstr.split(" ");

        pointx=plist.at(0);
        pointy=plist.at(1);
        point.setX((qreal)pointx.toFloat());
        point.setY((qreal)pointy.toFloat());
      //  qDebug()<<"in plist"<<plist<<pointx<<pointy<<point;
        pointlist.append(point);
    }
    polygon=QPolygonF::fromList(pointlist);

    //qDebug()<<"in plist"<<pointlist;
   // qDebug()<<"in plist"<<polygon;

    return polygon;
}


void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

    if (mouseEvent->button() != Qt::LeftButton)
        return;

    DiagramItem *item;
    QString itemid;
    QStringList roiStrList;
    QString everyPoint;
    static unsigned int idIndex=0;
    enum DiagramType { Step=0, Conditional, StartEnd, Io,RecEcl };
    switch (myMode) {
        case InsertItem://新增一个项目
            item = new DiagramItem(myItemType, myItemMenu);
            item->setBrush(myItemColor);
            item->setColor(myLineColor);
            addItem(item);
            item->setPos(mouseEvent->scenePos());
            qDebug() << "insert item at: " << mouseEvent->scenePos();
            qDebug() << "\ttype: " << myItemType << " color: " << myItemColor;
            emit itemInserted(item);//发送已经添加项目完成信号

            hasItemSelected = itemAt(mouseEvent->scenePos(), QTransform()) != nullptr;

            if(myItemType==Step) itemid="Step_"+QString::number(idIndex);
            else if(myItemType==Conditional) itemid="Conditional_"+QString::number(idIndex);
            else if(myItemType==StartEnd) itemid="StartEnd_"+QString::number(idIndex);
            else if(myItemType==Io) itemid="Io_"+QString::number(idIndex);
            else if(myItemType==RecEcl) itemid="RecEcl_"+QString::number(idIndex);
            else itemid="unknown_"+QString::number(idIndex);
            idIndex++;
            item->itemid=itemid;
            if(itemid=="RecEcl")
            {
           // qDebug()<<getNodeAttribute("ROI","id");//获取ROI的ID属性值
            setNodeAttribute("ROI","id",itemid);
            setNodeAttribute("ROI","x1",QString::number(item->mapRectToScene(item->boundingRect()).x()));
            setNodeAttribute("ROI","y1",QString::number(item->mapRectToScene(item->boundingRect()).y()));
            setNodeAttribute("ROI","x2",QString::number(item->mapRectToScene(item->boundingRect()).x()+ item->mapRectToScene(item->boundingRect()).width()));
            setNodeAttribute("ROI","y2",QString::number(item->mapRectToScene(item->boundingRect()).y()+ item->mapRectToScene(item->boundingRect()).height()));
            setNodeAttribute("ROI","polygon",polygonToQstring(item->polygon()));

            //qDebug()<<polygonToQstring(item->polygon());
            //qDebug()<<qstringToPolygon(polygonToQstring(item->polygon()));

            qDebug()<<doc.toString();
            }

            break;
        case InsertLine:
            if (itemAt(mouseEvent->scenePos(), QTransform()) == nullptr) break;
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
            addItem(line);
            break;

        case InsertText:
            textItem = new DiagramTextItem();
            textItem->setFont(myFont);
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            textItem->setZValue(1000.0);
            connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)),
                    this, SLOT(editorLostFocus(DiagramTextItem*)));
            connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
                    this, SIGNAL(itemSelected(QGraphicsItem*)));
            addItem(textItem);
            textItem->setDefaultTextColor(myTextColor);
            textItem->setPos(mouseEvent->scenePos());
            emit textInserted(textItem);
            qDebug() << "text inserted at" << textItem->scenePos();
            break;

    default:
        hasItemSelected = itemAt(mouseEvent->scenePos(), QTransform()) != nullptr;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);

    update();
}
#if 0
void DiagramScene::keyPressEvent(QKeyEvent *event)
{
    if(event->key()  ==  Qt::Key_R){
        if(this->backgroundPixmap.isNull() != true){
            QImage images(this->backgroundPixmap.toImage());
            QMatrix matrix;
            matrix.rotate(-180.0);//逆时针旋转90度
            images= images.transformed(matrix,Qt::FastTransformation);
            this->backgroundPixmap.fromImage(images);

            this->clear();
            QGraphicsPixmapItem* _backGroundItem= this->addPixmap(this->backgroundPixmap);
            QRect mainRect(0,0,this->backgroundPixmap.width(),this->backgroundPixmap.height());
            this->setSceneRect(mainRect);//设置场景范围
            _backGroundItem->setPos(0,0);


            update();
            qDebug()<<"in the key press and change dir 180";

        }
    }
   // qDebug()<<"key_press"<<event->key();
    QGraphicsScene::keyPressEvent(event);
}

#endif

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if (myMode == InsertLine && line != nullptr) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (myMode == MoveItem) {
        if (hasItemSelected)
            mouseDraggingMoveEvent(mouseEvent);
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }

    update();
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    hasItemSelected = false;

    // leave sticky mode
    horizontalStickyMode = false;
    verticalStickyMode = false;
    foreach(QGraphicsItem* p, selectedItems())
        p->setFlag(QGraphicsItem::ItemIsMovable);

    clearOrthogonalLines();
    if (line != nullptr && myMode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;
    }

    line = nullptr;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
    update();


}

void DiagramScene::wheelEvent(QGraphicsSceneWheelEvent* wheelEvent) {
    // ctrl key is being pressed
    if ((wheelEvent->modifiers() & Qt::KeyboardModifier::ControlModifier) != 0) {
        emit scaleChanging(wheelEvent->delta());
        wheelEvent->accept();
    } else {
        QGraphicsScene::wheelEvent(wheelEvent);
    }
    update();
}

void DiagramScene::mouseDraggingMoveEvent(QGraphicsSceneMouseEvent* event) {
    clearOrthogonalLines();
    if ((event->buttons() & Qt::LeftButton) != 0 && selectedItems().size() == 1) {
        QGraphicsItem* itemUnderCursor = selectedItems().first();
        QPointF curCenter = itemUnderCursor->scenePos();
        QPointF const& mousePos = event->scenePos();

        foreach(QGraphicsItem* p, items()) {
            if (p->type() != DiagramItem::Type || p == itemUnderCursor) continue;

            DiagramItem* item = qgraphicsitem_cast<DiagramItem*>(p);
            QPointF const& objPoint = item->scenePos();
            LineAttr lineAttr;

            tryEnteringStickyMode(itemUnderCursor, objPoint, mousePos);

            if ((lineAttr = getPointsRelationship(objPoint, curCenter)) != Other) {
                if ((lineAttr & Horizontal) != 0) {
                    QGraphicsLineItem* newHLine = new QGraphicsLineItem();
                    newHLine->setLine(QLineF(QPointF(0, objPoint.y()),
                                             QPointF(sceneRect().width(), objPoint.y())));
                    newHLine->setPen(penForLines);
                    orthogonalLines.append(newHLine);
                }
                if ((lineAttr & Vertical) != 0) {
                    QGraphicsLineItem* newVLine = new QGraphicsLineItem();
                    newVLine->setLine(QLineF(QPointF(objPoint.x(), 0),
                                             QPointF(objPoint.x(), sceneRect().height())));
                    newVLine->setPen(penForLines);
                    orthogonalLines.append(newVLine);
                }
            }
        }
        tryLeavingStickyMode(itemUnderCursor, mousePos);
    }
    foreach(QGraphicsLineItem* p, orthogonalLines) {
        addItem(p);
    }
    update();
}

void DiagramScene::clearOrthogonalLines() {
    foreach(QGraphicsLineItem* p, orthogonalLines) {
        removeItem(p);
        delete p;
    }
    orthogonalLines.clear();
}

bool DiagramScene::closeEnough(qreal x, qreal y, qreal delta) {
    return std::abs(x - y) < delta;
}

DiagramScene::LineAttr DiagramScene::getPointsRelationship(const QPointF& p1,
                                                           const QPointF& p2) {
    int ret = Other;
    ret |= closeEnough(p1.x(), p2.x(), Delta) ? Vertical : Other;
    ret |= closeEnough(p1.y(), p2.y(), Delta) ? Horizontal : Other;
    return static_cast<DiagramScene::LineAttr>(ret);
}

void DiagramScene::tryEnteringStickyMode(QGraphicsItem* item, const QPointF& target,
                                         const QPointF& mousePos) {
    QPointF const& itemPos = item->scenePos();
    if (!verticalStickyMode) {
        if (closeEnough(itemPos.x(), target.x(), stickyDistance)) {  // enter stickyMode
            verticalStickyMode = true;
            verticalStickPoint = mousePos;
            item->setFlag(QGraphicsItem::ItemIsMovable, false);
            item->setPos(QPointF(target.x(), itemPos.y()));
        }
    }
    if (!horizontalStickyMode) {
        if (closeEnough(itemPos.y(), target.y(), stickyDistance)) {
            horizontalStickyMode = true;
            horizontalStickPoint = mousePos;
            item->setFlag(QGraphicsItem::ItemIsMovable, false);
            item->setPos(QPointF(itemPos.x(), target.y()));
        }
    }
}

void DiagramScene::tryLeavingStickyMode(QGraphicsItem* item, const QPointF& mousePos) {
    if (verticalStickyMode) { // already in stickyMode, item should be able to move vertically
        item->moveBy(0, mousePos.y() - verticalStickPoint.y());
        verticalStickPoint.setY(mousePos.y());

        // when to exit stickyMode?
        if (!closeEnough(mousePos.x(), verticalStickPoint.x(), stickyDistance)) {
            verticalStickyMode = false;
            item->setFlag(QGraphicsItem::ItemIsMovable, true);
        }
    }
    if (horizontalStickyMode) {
        item->moveBy(mousePos.x() - horizontalStickPoint.x(), 0);
        horizontalStickPoint.setX(mousePos.x());

        if (!closeEnough(mousePos.y(), horizontalStickPoint.y(), stickyDistance)) {
            horizontalStickyMode = false;
            item->setFlag(QGraphicsItem::ItemIsMovable, true);
        }
    }
}

