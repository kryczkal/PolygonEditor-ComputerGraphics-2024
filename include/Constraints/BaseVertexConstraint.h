//
// Created by wookie on 10/21/24.
//

#ifndef POLYGONEDITOR_VERTEXCONSTRAINT_H
#define POLYGONEDITOR_VERTEXCONSTRAINT_H
#include <QPainter>

class PolygonVertexItem;
class ControlVertexItem;
class PolygonEdgeItem;
class BezierEdgeItem;

class BaseVertexConstraint {
public:
    virtual void apply(PolygonVertexItem *vertexItem)                      = 0;
    virtual void apply(ControlVertexItem *controlVertexItem) = 0;
    virtual void paintIcon(QPainter *painter, const QPointF &center, const PolygonVertexItem *) = 0;
    virtual ~BaseVertexConstraint() = default;

    friend class BezierEdgeItem;
};


#endif //POLYGONEDITOR_VERTEXCONSTRAINT_H
