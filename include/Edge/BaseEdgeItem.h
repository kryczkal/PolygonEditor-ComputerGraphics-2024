//
// Created by wookie on 10/11/24.
//

#ifndef POLYGONEDITOR_BASEEDGEITEM_H
#define POLYGONEDITOR_BASEEDGEITEM_H

#include <QAction>
#include <QGraphicsItem>
#include <QList>
#include <QPainter>
#include <gtest/gtest.h>

class BaseVertexItem;
class BaseEdgeConstraint;

class BaseEdgeItem : public QGraphicsItem
{
    public:
    BaseEdgeItem(BaseVertexItem *start, BaseVertexItem *end, QGraphicsItem *parent = nullptr);

    virtual QRectF boundingRect() const                                                             = 0;
    virtual void paintIndex(QPainter *painter, QPointF startPoint, QPointF endPoint, int edgeIndex) = 0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)  = 0;

    BaseVertexItem *getStartVertex() const;
    BaseVertexItem *getEndVertex() const;

    virtual ~BaseEdgeItem();

    [[nodiscard]] virtual BaseEdgeConstraint *getConstraint() const = 0;

    protected:
    BaseVertexItem *startVertex;
    BaseVertexItem *endVertex;
    qreal width = 2.0;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) = 0;
    friend class PolygonItem;
};

#endif // POLYGONEDITOR_BASEEDGEITEM_H
