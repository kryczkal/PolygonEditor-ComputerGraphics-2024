//
// Created by wookie on 10/21/24.
//

#ifndef POLYGONEDITOR_BASEVERTEXITEM_H
#define POLYGONEDITOR_BASEVERTEXITEM_H

#include <QGraphicsItem>
#include <QList>
#include <QPainter>
#include <gtest/gtest.h>

class PolygonEdgeItem;
class BaseEdgeItem;

class BaseVertexItem : public QGraphicsItem
{
    public:
    explicit BaseVertexItem(const QPointF &position, QGraphicsItem *parent = nullptr);

    [[nodiscard]] QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    BaseEdgeItem *edgeOut = nullptr;
    BaseEdgeItem *edgeIn  = nullptr;

    [[nodiscard]]
    bool hasBothEdges() const;
    [[nodiscard]] bool hasOneEdge() const;

    qreal radius = 5.0;

    protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    private:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override = 0;
    friend class PolygonItem;
};

#endif // POLYGONEDITOR_BASEVERTEXITEM_H
