//
// Created by wookie on 10/21/24.
//

#ifndef POLYGONEDITOR_CONTROLVERTEXITEM_H
#define POLYGONEDITOR_CONTROLVERTEXITEM_H
#include "Vertex/BaseVertexItem.h"

class ControlVertexItem : public BaseVertexItem {
public:
    explicit ControlVertexItem(const QPointF &position, QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    friend class PolygonItem;
    friend class BezierEdgeItem;
};


#endif //POLYGONEDITOR_CONTROLVERTEXITEM_H
