//
// Created by wookie on 10/11/24.
//

#ifndef POLYGONEDITOR_VERTEXITEM_H
#define POLYGONEDITOR_VERTEXITEM_H

#include <QGraphicsItem>
#include <QList>
#include <QPainter>
#include <gtest/gtest.h>

class EdgeItem; // Forward declaration

class VertexItem : public QGraphicsItem
{
    public:
    VertexItem(const QPointF &position, QGraphicsItem *parent = nullptr);

    [[nodiscard]] QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    EdgeItem* edgeFrom;
    EdgeItem* edgeTo;
    bool hasBothEdges() const;

    QPointF position;
    qreal radius = 5.0;

    private:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

    friend QDataStream &operator<<(QDataStream &out, const VertexItem &vertex);
    friend class PolygonItem;
};

#endif // POLYGONEDITOR_VERTEXITEM_H
