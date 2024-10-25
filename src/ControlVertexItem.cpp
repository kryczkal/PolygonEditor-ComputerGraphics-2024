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
#include <QDebug>
#include "Vertex/ControlVertexItem.h"
#include "Vertex/PolygonVertexItem.h"
#include "Edge/BaseEdgeItem.h"
#include "Edge/BezierEdgeItem.h"
#include "Constraints/BaseVertexConstraint.h"

ControlVertexItem::ControlVertexItem(const QPointF &position, QGraphicsItem *parent) : BaseVertexItem(position,
                                                                                                      parent) {
}

void ControlVertexItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setBrush(Qt::red);
    painter->drawEllipse(QPointF(0, 0), radius, radius);
}

void ControlVertexItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
}

void ControlVertexItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    BaseVertexItem::mouseMoveEvent(event);
    if (edgeIn) {
        qDebug() << "Constraint applied";
        PolygonVertexItem* castedVertex = dynamic_cast<PolygonVertexItem*>(edgeIn->getStartVertex());
        if (castedVertex->getConstraint()) castedVertex->getConstraint()->apply(this);
    }
    else if (edgeOut) {
        PolygonVertexItem* castedVertex = dynamic_cast<PolygonVertexItem*>(edgeOut->getEndVertex());
        qDebug() << "Constraint applied";
        if (castedVertex->getConstraint()) castedVertex->getConstraint()->apply(this);
    } else {
        qDebug() << "Warning: no edgeIn or edgeOut";
    }
}
