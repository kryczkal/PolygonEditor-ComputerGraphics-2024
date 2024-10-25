//
// Created by wookie on 10/11/24.
//

#ifndef POLYGONEDITOR_POLYGONVERTEXITEM_H
#define POLYGONEDITOR_POLYGONVERTEXITEM_H

#include <QGraphicsItem>
#include <QList>
#include <QPainter>
#include <gtest/gtest.h>
#include "Vertex/BaseVertexItem.h"
#include "Constraints/BaseVertexConstraint.h"

class BaseEdgeItem;
/*
 * @brief Class representing a vertex in the polygon
 * @details The class is responsible for drawing the vertex on the scene and handling the context menu
 * @param position - position of the vertex
 * @param parent - parent item - (It is required that the parent item is a PolygonItem)
 */
class PolygonVertexItem : public BaseVertexItem
{
    public:
    explicit PolygonVertexItem(const QPointF &position, QGraphicsItem *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    BaseVertexConstraint *getConstraint() const;

    protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    private:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    BaseVertexConstraint* constraint = nullptr;

    friend QDataStream &operator<<(QDataStream &out, const PolygonVertexItem &vertex);
    friend class PolygonItem;
};

#endif // POLYGONEDITOR_POLYGONVERTEXITEM_H
