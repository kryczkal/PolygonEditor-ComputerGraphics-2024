//
// Created by wookie on 10/21/24.
//

#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QPainter>
#include "Edge/BezierEdgeItem.h"
#include "PolygonItem.h"
#include "Vertex/ControlVertexItem.h"
#include "Edge/StandaloneEdgeItem.h"

BezierEdgeItem::BezierEdgeItem(PolygonVertexItem *start, PolygonVertexItem *end, QGraphicsItem *parent) : BaseEdgeItem(start, end,
                                                                                                                       parent) {
    Q_ASSERT(dynamic_cast<PolygonItem *>(parent) != nullptr); // Parent must be a PolygonItem

    QPointF startControlPointPos = start->edgeIn ? start->pos() + (start->pos() - start->edgeIn->getStartVertex()->pos()) : (start->pos() + end->pos()) / 2;
    QPointF endControlPointPos = end->edgeOut ? end->pos() + (end->pos() - end->edgeOut->getEndVertex()->pos()) : (start->pos() + end->pos()) / 2;

    startControlPoint = new ControlVertexItem(startControlPointPos, this);
    endControlPoint = new ControlVertexItem(endControlPointPos, this);

    startControlEdge = new StandaloneEdgeItem(start, startControlPoint, this);
    endControlEdge = new StandaloneEdgeItem(endControlPoint, end, this);

    startControlPoint->edgeIn = startControlEdge;
    endControlPoint->edgeOut = endControlEdge;

    Q_ASSERT(startControlEdge != nullptr);
    Q_ASSERT(endControlEdge != nullptr);

    Q_ASSERT(startControlPoint != nullptr);
    Q_ASSERT(endControlPoint != nullptr);
}
BezierEdgeItem::BezierEdgeItem(BaseVertexItem *start, BaseVertexItem *end, QGraphicsItem *parent) : BaseEdgeItem(start,
                                                                                                                 end,
                                                                                                                 parent) {
    Q_ASSERT(dynamic_cast<PolygonVertexItem *>(start) != nullptr);
    Q_ASSERT(dynamic_cast<PolygonVertexItem *>(end) != nullptr);
    Q_ASSERT(dynamic_cast<PolygonItem *>(parent) != nullptr); // Parent must be a PolygonItem

    QPointF startControlPointPos = start->edgeIn ? start->pos() + (start->pos() - start->edgeIn->getStartVertex()->pos()) : (start->pos() + end->pos()) / 2;
    QPointF endControlPointPos = end->edgeOut ? end->pos() + (end->pos() - end->edgeOut->getEndVertex()->pos()) : (start->pos() + end->pos()) / 2;

    startControlPoint = new ControlVertexItem(startControlPointPos, this);
    endControlPoint = new ControlVertexItem(endControlPointPos, this);

    startControlEdge = new StandaloneEdgeItem(start, startControlPoint, this->parentItem());
    endControlEdge = new StandaloneEdgeItem(endControlPoint, end, this->parentItem());

    startControlPoint->edgeIn = startControlEdge;
    endControlPoint->edgeOut = endControlEdge;

    Q_ASSERT(startControlEdge != nullptr);
    Q_ASSERT(endControlEdge != nullptr);

    Q_ASSERT(startControlPoint != nullptr);
    Q_ASSERT(endControlPoint != nullptr);
}

QRectF BezierEdgeItem::boundingRect() const {
    int minX = std::min({startVertex->pos().x(), endVertex->pos().x(), startControlPoint->pos().x(), endControlPoint->pos().x()});
    int minY = std::min({startVertex->pos().y(), endVertex->pos().y(), startControlPoint->pos().y(), endControlPoint->pos().y()});
    int maxX = std::max({startVertex->pos().x(), endVertex->pos().x(), startControlPoint->pos().x(), endControlPoint->pos().x()});
    int maxY = std::max({startVertex->pos().y(), endVertex->pos().y(), startControlPoint->pos().y(), endControlPoint->pos().y()});
    return QRectF(QPointF(minX, minY), QPointF(maxX, maxY)).normalized().adjusted(-2, -2, 2, 2);
}

void BezierEdgeItem::paintIndex(QPainter *painter, QPointF startPoint, QPointF endPoint, int edgeIndex) {
    // TODO: Implement paintIndex
}

void BezierEdgeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setPen(QPen(Qt::black, width));

    QPointF P0 = startVertex->pos();
    QPointF P1 = startControlPoint->pos();
    QPointF P2 = endControlPoint->pos();
    QPointF P3 = endVertex->pos();

    // Calculate coefficients for the power basis form of the Bezier curve
    QPointF A = -P0 + 3 * P1 - 3 * P2 + P3;
    QPointF B = 3 * P0 - 6 * P1 + 3 * P2;
    QPointF C = -3 * P0 + 3 * P1;
    QPointF D = P0;

    double dt = 1.0 / BezierEdgeItem::DRAW_ITERATION_STEPS;

    QPointF prevPoint = D;
    for (int i = 1; i <= BezierEdgeItem::DRAW_ITERATION_STEPS; ++i) {
        double t = i * dt;
        QPointF currentPoint = A * (t * t * t) + B * (t * t) + C * t + D;
        painter->drawLine(prevPoint, currentPoint);
        prevPoint = currentPoint;
    }
}

void BezierEdgeItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    QMenu menu;
    QAction *subdivideAction = menu.addAction("Subdivide");
    QIODevice::connect(
            subdivideAction, &QAction::triggered,
            [this]
            {
                PolygonItem *polygon = dynamic_cast<PolygonItem *>(parentItem());
                polygon->subdivideEdge(this);
            }
    );

    QAction *changeToNormalAction;

    changeToNormalAction = menu.addAction("Change to Normal edge");
    QIODevice::connect(
            changeToNormalAction, &QAction::triggered,
            [this]
            {
                PolygonItem *polygon = dynamic_cast<PolygonItem *>(parentItem());
                polygon->changeEdgeToNormal(this);
            }
    );

    menu.exec(event->screenPos());
}

BaseEdgeConstraint *BezierEdgeItem::getConstraint() const {
    return nullptr;
}

BezierEdgeItem::~BezierEdgeItem() {
    delete startControlPoint;
    delete endControlPoint;
    delete startControlEdge;
    delete endControlEdge;
}

