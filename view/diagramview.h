#ifndef DIAGRAMVIEW_H
#define DIAGRAMVIEW_H
#include <QGraphicsView>
#include "diagramitem.h"

class DiagramView : public QGraphicsView {
    Q_OBJECT
public:
    DiagramView(QGraphicsScene *scene, QWidget *parent = Q_NULLPTR);
    DiagramView(QWidget* parent);
    void fitView();
signals:
    void needsUndoBackup();
    void dragEnd(QString id,QRectF bdRec,QPolygonF polygon);
    void moveEnd(QString id,QRectF bdRec,QPolygonF polygon);
protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};

#endif // DIAGRAMVIEW_H
