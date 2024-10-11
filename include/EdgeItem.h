//
// Created by wookie on 10/11/24.
//

#ifndef POLYGONEDITOR_EDGEITEM_H
#define POLYGONEDITOR_EDGEITEM_H

#include <QGraphicsItem>
#include <QList>
#include <QPainter>

#include "VertexItem.h"

class EdgeItem : public QGraphicsItem
{
    public:
    EdgeItem(VertexItem *start, VertexItem *end, QGraphicsItem *parent = nullptr);

    [[nodiscard]] QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    private:
    void bresenham(QPainter *painter, const QPointF &start, const QPointF &end, qreal width);

    VertexItem *startVertex;
    VertexItem *endVertex;
    qreal width = 2.0;

    static constexpr bool useBresenham = true;
};

#endif // POLYGONEDITOR_EDGEITEM_H
