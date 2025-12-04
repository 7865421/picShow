#include "diagramview.h"
#include <QKeyEvent>
#include <QDebug>
#include "diagramitem.h"
#include "diagramtextitem.h"

DiagramView::DiagramView(QGraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent) {
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    setDragMode(QGraphicsView::RubberBandDrag);
}
DiagramView::DiagramView(QWidget* parent)
     {
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    setDragMode(QGraphicsView::RubberBandDrag);
}
void DiagramView::keyPressEvent(QKeyEvent* event) {
    if ((event->modifiers() & Qt::KeyboardModifier::ControlModifier) != 0) {
        setDragMode(DragMode::ScrollHandDrag);
    }
  //键盘事件

    QGraphicsView::keyPressEvent(event);
}

void DiagramView::keyReleaseEvent(QKeyEvent* event) {
    if ((event->modifiers() & Qt::KeyboardModifier::ControlModifier) == 0) {
        setDragMode(DragMode::RubberBandDrag);
    }
}

void DiagramView::mouseReleaseEvent(QMouseEvent* event) {
    QGraphicsView::mouseReleaseEvent(event);
    bool needsEmit = false;
    foreach(QGraphicsItem* item, scene()->selectedItems()) {
        if (item->type() == DiagramItem::Type) {
            DiagramItem* p = qgraphicsitem_cast<DiagramItem*>(item);
            if (p->isMoved) {
                needsEmit = true;
                p->isMoved = false;
                //qDebug()<<"移动完毕"<<item->mapRectToScene(item->boundingRect());
                emit moveEnd(p->itemid,p->mapRectToScene(p->boundingRect()),p->myPolygon);

            }
            if (p->isResized) {
                needsEmit = true;
                p->isResized = false;
               // qDebug()<<"调整大小完毕"<<item->mapRectToScene(item->boundingRect());
                emit moveEnd(p->itemid,p->mapRectToScene(p->boundingRect()),p->myPolygon);
            }

        } else if (item->type() == DiagramTextItem::Type) {
            DiagramTextItem* p = qgraphicsitem_cast<DiagramTextItem*>(item);
            if (p->positionIsUpdated()) {
                qDebug() << "isUpdated!";
                needsEmit = true;
                p->setUpdated();
            }
        }
    }
    if (needsEmit) emit needsUndoBackup();
  //  setCursor(Qt::ArrowCursor);
}

void DiagramView::fitView()
{
    QRectF rectItem = scene()->itemsBoundingRect();
    QRectF rectView = this->rect();
    qreal ratioView = rectView.height() / rectView.width();
    qreal ratioItem = rectItem.height() / rectItem.width();
    if (ratioView > ratioItem)
    {
        rectItem.moveTop(rectItem.width()*ratioView - rectItem.height());
        rectItem.setHeight(rectItem.width()*ratioView);

        rectItem.setWidth(rectItem.width() * 1.2);
        rectItem.setHeight(rectItem.height() * 1.2);
    }
    else
    {
        rectItem.moveLeft(rectItem.height()/ratioView - rectItem.width());
        rectItem.setWidth(rectItem.height()/ratioView);

        rectItem.setWidth(rectItem.width() * 1.2);
        rectItem.setHeight(rectItem.height() * 1.2);
    }
    this->setSceneRect(this->sceneRect());
    qDebug()<<"panview"<<this->sceneRect();
   // this->fitInView(rectItem, Qt::KeepAspectRatio);
    //this->fitInView(this->sceneRect(),Qt::IgnoreAspectRatio );
    this->fitInView(this->sceneRect(),Qt::KeepAspectRatio );
}
