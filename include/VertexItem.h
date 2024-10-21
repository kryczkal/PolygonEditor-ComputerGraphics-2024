//
// Created by wookie on 10/11/24.
//

#ifndef POLYGONEDITOR_VERTEXITEM_H
#define POLYGONEDITOR_VERTEXITEM_H

#include <QGraphicsItem>
#include <QList>
#include <QPainter>
#include <gtest/gtest.h>

class EdgeItemNormal;
/*
 * @brief Class representing a vertex in the polygon
 * @details The class is responsible for drawing the vertex on the scene and handling the context menu
 * @param position - position of the vertex
 * @param parent - parent item - (It is required that the parent item is a PolygonItem)
 */
class VertexItem : public QGraphicsItem
{
    public:
    explicit VertexItem(const QPointF &position, QGraphicsItem *parent = nullptr);

    [[nodiscard]] QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    EdgeItemNormal *edgeOut = nullptr;
    EdgeItemNormal *edgeIn  = nullptr;

    [[nodiscard]]
    bool hasBothEdges() const;
    bool hasOneEdge() const;

    qreal radius = 5.0;

    protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    private:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

    friend QDataStream &operator<<(QDataStream &out, const VertexItem &vertex);
    friend class PolygonItem;
};

#endif // POLYGONEDITOR_VERTEXITEM_H
