//
// Created by wookie on 10/21/24.
//

#include "Constraints/G1VertexConstraint.h"
#include "Vertex/PolygonVertexItem.h"

//
// Created by wookie on 10/21/24.
//

#include "Constraints/C1VertexConstraint.h"
#include "Edge/BezierEdgeItem.h"
#include "Edge/PolygonEdgeItem.h"
#include "Vertex/ControlVertexItem.h"
#include "Vertex/PolygonVertexItem.h"
#include <QVector2D>

void G1VertexConstraint::apply(PolygonVertexItem *vertexItem)
{
    BezierEdgeItem *castEdgeOut = dynamic_cast<BezierEdgeItem *>(vertexItem->edgeOut);
    BezierEdgeItem *castEdgeIn  = dynamic_cast<BezierEdgeItem *>(vertexItem->edgeIn);

    if (castEdgeOut)
        applyG1ContinuityToOutgoingEdge(vertexItem, castEdgeOut);
    if (castEdgeIn)
        applyG1ContinuityToIncomingEdge(vertexItem, castEdgeIn);
}

void G1VertexConstraint::apply(ControlVertexItem *controlVertexItem)
{
    BezierEdgeItem *castedParent = dynamic_cast<BezierEdgeItem *>(controlVertexItem->parentItem());
    Q_ASSERT(castedParent != nullptr);
    if (controlVertexItem == castedParent->startControlPoint)
        apply(reinterpret_cast<PolygonVertexItem *>(castedParent->getStartVertex()));
    if (controlVertexItem == castedParent->endControlPoint)
        apply(reinterpret_cast<PolygonVertexItem *>(castedParent->getEndVertex()));
}

void G1VertexConstraint::applyG1ContinuityToOutgoingEdge(PolygonVertexItem *vertexItem, BezierEdgeItem *castEdgeOut)
{
    QPointF startPos = vertexItem->edgeIn->getStartVertex()->pos();
    QPointF endPos   = vertexItem->edgeIn->getEndVertex()->pos();

    QPointF newControlPointPos = calculateParallelControlPoint(startPos, endPos, castEdgeOut->startControlPoint->pos());
    castEdgeOut->startControlPoint->setPos(newControlPointPos);
}

void G1VertexConstraint::applyG1ContinuityToIncomingEdge(PolygonVertexItem *vertexItem, BezierEdgeItem *castEdgeIn)
{
    QPointF startPos = vertexItem->edgeOut->getStartVertex()->pos();
    QPointF endPos   = vertexItem->edgeOut->getEndVertex()->pos();

    QPointF newControlPointPos = calculateParallelControlPoint(endPos, startPos, castEdgeIn->endControlPoint->pos());
    castEdgeIn->endControlPoint->setPos(newControlPointPos);
}

QPointF G1VertexConstraint::calculateParallelControlPoint(
    const QPointF &basePos, const QPointF &targetPos, const QPointF &currentControlPointPos
)
{
    QVector2D edgeDirection(targetPos - basePos);
    QVector2D controlVector(currentControlPointPos - basePos);

    QVector2D unitEdgeDirection     = edgeDirection.normalized();
    double projectionLength         = QVector2D::dotProduct(controlVector, unitEdgeDirection);
    QVector2D parallelControlVector = unitEdgeDirection * projectionLength;

    QPointF parallelControlPoint = basePos + parallelControlVector.toPointF();

    double edgeLength               = edgeDirection.length();
    double distanceToProjectedPoint = QVector2D(parallelControlPoint - basePos).length();

    if (distanceToProjectedPoint <= edgeLength)
    {
        return targetPos;
    }

    return parallelControlPoint;
}
void G1VertexConstraint::paintIcon(QPainter *painter, const QPointF &center, const PolygonVertexItem *) {}
