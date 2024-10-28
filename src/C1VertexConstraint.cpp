//
// Created by wookie on 10/21/24.
//

#include "Constraints/C1VertexConstraint.h"
#include "Edge/BezierEdgeItem.h"
#include "Edge/PolygonEdgeItem.h"
#include "Vertex/ControlVertexItem.h"
#include "Vertex/PolygonVertexItem.h"
#include <QVector2D>

void C1VertexConstraint::apply(PolygonVertexItem *vertexItem)
{
    BezierEdgeItem *castEdgeOut = dynamic_cast<BezierEdgeItem *>(vertexItem->edgeOut);
    BezierEdgeItem *castEdgeIn  = dynamic_cast<BezierEdgeItem *>(vertexItem->edgeIn);

    if (castEdgeOut)
        applyC1ContinuityToOutgoingEdge(vertexItem, castEdgeOut);
    if (castEdgeIn)
        applyC1ContinuityToIncomingEdge(vertexItem, castEdgeIn);
}

void C1VertexConstraint::apply(ControlVertexItem *controlVertexItem)
{
    BezierEdgeItem *castedParent = dynamic_cast<BezierEdgeItem *>(controlVertexItem->parentItem());
    Q_ASSERT(castedParent != nullptr);
    if (controlVertexItem == castedParent->startControlPoint)
        apply(reinterpret_cast<PolygonVertexItem *>(castedParent->getStartVertex()));
    if (controlVertexItem == castedParent->endControlPoint)
        apply(reinterpret_cast<PolygonVertexItem *>(castedParent->getEndVertex()));
}

void C1VertexConstraint::applyC1ContinuityToOutgoingEdge(PolygonVertexItem *vertexItem, BezierEdgeItem *castEdgeOut)
{
    QPointF startPos = vertexItem->edgeIn->getStartVertex()->pos();
    QPointF endPos   = vertexItem->edgeIn->getEndVertex()->pos();

    QPointF newControlPointPos = calculateControlPointPosition(startPos, endPos, 1.0 / 3.0);
    castEdgeOut->startControlPoint->setPos(newControlPointPos);
}

void C1VertexConstraint::applyC1ContinuityToIncomingEdge(PolygonVertexItem *vertexItem, BezierEdgeItem *castEdgeIn)
{
    QPointF startPos = vertexItem->edgeOut->getStartVertex()->pos();
    QPointF endPos   = vertexItem->edgeOut->getEndVertex()->pos();

    QPointF newControlPointPos = calculateControlPointPosition(endPos, startPos, 1.0 / 3.0);

    castEdgeIn->endControlPoint->setPos(newControlPointPos);
}

QPointF
C1VertexConstraint::calculateControlPointPosition(const QPointF &basePos, const QPointF &targetPos, double fraction)
{
    QVector2D directionVector(targetPos - basePos);

    double edgeLength = directionVector.length();

    QVector2D unitDirection = directionVector.normalized();

    QVector2D scaledVector = unitDirection * (edgeLength * fraction);

    return targetPos + scaledVector.toPointF();
}

void C1VertexConstraint::paintIcon(QPainter *painter, const QPointF &center, const PolygonVertexItem *) {}
