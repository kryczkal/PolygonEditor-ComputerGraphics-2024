//
// Created by wookie on 10/21/24.
//

#include <QAction>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>
#include "Vertex/BaseVertexItem.h"

BaseVertexItem::BaseVertexItem(const QPointF &position, QGraphicsItem *parent) : QGraphicsItem(parent) {
    setPos(position);
    setZValue(1);
}

QRectF BaseVertexItem::boundingRect() const {
    return QRectF(-radius, -radius, 2 * radius, 2 * radius);
}

bool BaseVertexItem::hasBothEdges() const {
    return edgeOut != nullptr && edgeIn != nullptr;
}

bool BaseVertexItem::hasOneEdge() const {
    return !hasBothEdges() && (edgeOut != nullptr || edgeIn != nullptr);
}

void BaseVertexItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setBrush(Qt::black);
    painter->drawEllipse(QPointF(0, 0), radius, radius);
}

void BaseVertexItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    setFlag(QGraphicsItem::ItemIsMovable);
}

void BaseVertexItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QPointF delta = event->pos() - event->lastPos();
    moveBy(delta.x(), delta.y());

    prepareGeometryChange();
    scene()->update();
}

void BaseVertexItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    setFlag(QGraphicsItem::ItemIsMovable, false);
}
