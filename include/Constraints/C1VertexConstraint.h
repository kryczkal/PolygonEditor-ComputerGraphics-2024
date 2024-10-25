//
// Created by wookie on 10/21/24.
//

#ifndef POLYGONEDITOR_C1VERTEXCONSTRAINT_H
#define POLYGONEDITOR_C1VERTEXCONSTRAINT_H

#include "BaseVertexConstraint.h"

class C1VertexConstraint : public BaseVertexConstraint {
    void apply(PolygonVertexItem *vertexItem) override;
    void apply(ControlVertexItem *controlVertexItem) override;
    void paintIcon(QPainter *painter, const QPointF &center, const PolygonVertexItem *) override;
    friend class BezierEdgeItem;

    static void applyC1ContinuityToIncomingEdge(PolygonVertexItem *vertexItem, BezierEdgeItem *castEdgeIn);

    static void applyC1ContinuityToOutgoingEdge(PolygonVertexItem *vertexItem, BezierEdgeItem *castEdgeOut);

    static QPointF calculateControlPointPosition(const QPointF &basePos, const QPointF &targetPos, double fraction);
};


#endif //POLYGONEDITOR_C1VERTEXCONSTRAINT_H
