//
// Created by wookie on 10/11/24.
//

#ifndef POLYGONEDITOR_EDGEITEM_H
#define POLYGONEDITOR_EDGEITEM_H

#include <QAction>
#include <QGraphicsItem>
#include <QList>
#include <QPainter>
#include <gtest/gtest.h>

class VertexItem;
class BaseEdgeConstraint;

/*
 * @brief Class representing an edge in the polygon
 * @details The class is responsible for drawing the edge on the scene and handling the context menu
 * @param start - start vertex of the edge
 * @param end - end vertex of the edge
 * @param parent - parent item - (It is required that the parent item is a PolygonItem)
 */
class EdgeItem : public QGraphicsItem
{
    public:
    EdgeItem(VertexItem *start, VertexItem *end, QGraphicsItem *parent = nullptr);

    [[nodiscard]] QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    VertexItem *getStartVertex() const;
    VertexItem *getEndVertex() const;

    void setConstraint(BaseEdgeConstraint *constraint);
    BaseEdgeConstraint *getConstraint() const;

    ~EdgeItem() override;

    private:
    static constexpr bool useBresenham = true;

    VertexItem *startVertex;
    VertexItem *endVertex;
    qreal width = 2.0;

    BaseEdgeConstraint *constraint = nullptr;

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    void bresenham(QPainter *painter, const QPointF &start, const QPointF &end, qreal width);

    friend QDataStream &operator<<(QDataStream &out, const EdgeItem &edge);
    friend class PolygonItem;

    void handleContextMenuAction(QAction *selectedAction);

    void createContextMenu(QMenu *menu);
};

#endif // POLYGONEDITOR_EDGEITEM_H
