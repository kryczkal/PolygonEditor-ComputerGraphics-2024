//
// Created by wookie on 10/21/24.
//

#ifndef POLYGONEDITOR_BEZIEREDGEITEM_H
#define POLYGONEDITOR_BEZIEREDGEITEM_H
#include "PolygonEdgeItem.h"

class ControlVertexItem;
class StandaloneEdgeItem;

class BezierEdgeItem : public BaseEdgeItem{
public:
    BezierEdgeItem(PolygonVertexItem *start, PolygonVertexItem *end, QGraphicsItem *parent = nullptr);
    BezierEdgeItem(BaseVertexItem *start, BaseVertexItem *end, QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const override;
    void paintIndex(QPainter *painter, QPointF startPoint, QPointF endPoint, int edgeIndex) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    BaseEdgeConstraint *getConstraint() const override;

    ~BezierEdgeItem() override;
protected:
    ControlVertexItem *startControlPoint = nullptr;
    ControlVertexItem *endControlPoint = nullptr;
    StandaloneEdgeItem *startControlEdge = nullptr;
    StandaloneEdgeItem *endControlEdge = nullptr;

    static constexpr int DRAW_ITERATION_STEPS = 100;

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    friend class PolygonItem;
    friend class BaseVertexConstraint;
    friend class C1VertexConstraint;
    friend class G0VertexConstraint;
    friend class G1VertexConstraint;
};

#endif //POLYGONEDITOR_BEZIEREDGEITEM_H
