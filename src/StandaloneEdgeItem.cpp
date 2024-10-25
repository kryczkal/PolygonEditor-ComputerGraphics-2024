//
// Created by wookie on 10/21/24.
//

#include "Edge/StandaloneEdgeItem.h"
#include "Vertex/BaseVertexItem.h"
#include "Edge/BaseEdgeItem.h"

StandaloneEdgeItem::StandaloneEdgeItem(BaseVertexItem *start, BaseVertexItem *end, QGraphicsItem *parent) : BaseEdgeItem(start, end, parent) {

}

QRectF StandaloneEdgeItem::boundingRect() const {
    QPointF startPoint = mapFromItem(startVertex, 0, 0);
    QPointF endPoint   = mapFromItem(endVertex, 0, 0);
    return QRectF(startPoint, endPoint).normalized().adjusted(-2, -2, 2, 2);
}

void StandaloneEdgeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QPointF startPoint = mapFromItem(startVertex, 0, 0);
    QPointF endPoint   = mapFromItem(endVertex, 0, 0);

    painter->setPen(QPen(Qt::black, width));
    painter->drawLine(startPoint, endPoint);
}

void StandaloneEdgeItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {

}

QDataStream &operator<<(QDataStream &out, const StandaloneEdgeItem &edge) {
    out << edge.startVertex->pos() << edge.endVertex->pos();
    return out;
}

void StandaloneEdgeItem::paintIndex(QPainter *painter, QPointF startPoint, QPointF endPoint, int edgeIndex) {

}

BaseEdgeConstraint *StandaloneEdgeItem::getConstraint() const {
    return nullptr;
}
