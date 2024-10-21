//
// Created by wookie on 10/11/24.
//

#ifndef POLYGONEDITOR_EDGEITEMBASE_H
#define POLYGONEDITOR_EDGEITEMBASE_H

#include <QAction>
#include <QGraphicsItem>
#include <QList>
#include <QPainter>
#include <gtest/gtest.h>

class VertexItem;
class BaseEdgeConstraint;

class EdgeItemBase : public QGraphicsItem
{
    public:
    EdgeItemBase(VertexItem *start, VertexItem *end, QGraphicsItem *parent = nullptr);

    virtual QRectF boundingRect() const                                                             = 0;
    virtual void paintIndex(QPainter *painter, QPointF startPoint, QPointF endPoint, int edgeIndex) = 0;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)  = 0;

    VertexItem *getStartVertex() const;
    VertexItem *getEndVertex() const;

    virtual ~EdgeItemBase();

    protected:
    VertexItem *startVertex;
    VertexItem *endVertex;
    qreal width = 2.0;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) = 0;
    friend class PolygonItem;
};

#endif // POLYGONEDITOR_EDGEITEMBASE_H
