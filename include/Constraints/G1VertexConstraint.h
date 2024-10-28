//
// Created by wookie on 10/21/24.
//

#ifndef POLYGONEDITOR_G1VERTEXCONSTRAINT_H
#define POLYGONEDITOR_G1VERTEXCONSTRAINT_H

#include "BaseVertexConstraint.h"

class G1VertexConstraint : public BaseVertexConstraint
{
    void apply(PolygonVertexItem *vertexItem) override;
    void apply(ControlVertexItem *controlVertexItem) override;
    void paintIcon(QPainter *painter, const QPointF &center, const PolygonVertexItem *) override;

    static void applyG1ContinuityToOutgoingEdge(PolygonVertexItem *vertexItem, BezierEdgeItem *castEdgeOut);

    static void applyG1ContinuityToIncomingEdge(PolygonVertexItem *vertexItem, BezierEdgeItem *castEdgeIn);

    static QPointF calculateParallelControlPoint(
        const QPointF &basePos, const QPointF &targetPos, const QPointF &currentControlPointPos
    );
};

#endif // POLYGONEDITOR_G1VERTEXCONSTRAINT_H
