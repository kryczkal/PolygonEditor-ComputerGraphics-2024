//
// Created by wookie on 10/11/24.
//

#ifndef POLYGONEDITOR_POLYGONEDGEITEM_H
#define POLYGONEDITOR_POLYGONEDGEITEM_H

#include "BaseEdgeItem.h"
#include <QAction>
#include <QGraphicsItem>
#include <QList>
#include <QPainter>
#include <gtest/gtest.h>

class PolygonVertexItem;
class BaseEdgeConstraint;

/*
 * @brief Class representing an edge in the polygon
 * @details The class is responsible for drawing the edge on the scene and handling the context menu
 * @param start - start vertex of the edge
 * @param end - end vertex of the edge
 * @param parent - parent item - (It is required that the parent item is a PolygonItem)
 */
class PolygonEdgeItem : public BaseEdgeItem
{
    public:
    PolygonEdgeItem(PolygonVertexItem *start, PolygonVertexItem *end, QGraphicsItem *parent = nullptr);
    PolygonEdgeItem(BaseVertexItem *start, BaseVertexItem *end, QGraphicsItem *parent = nullptr);

    [[nodiscard]] QRectF boundingRect() const override;
    void paintIndex(QPainter *painter, QPointF startPoint, QPointF endPoint, int edgeIndex) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void drawLine(QPainter *painter, QPointF startPoint, QPointF endPoint, bool useBresenham);

    [[maybe_unused]] void setConstraint(BaseEdgeConstraint *edgeConstraint);
    [[nodiscard]] BaseEdgeConstraint *getConstraint() const override;

    void addLengthConstraint();
    void addLengthConstraintNoWindow();
    void addHorizontalConstraint();
    void addVerticalConstraint();
    void makeBezier();
    void deleteConstraint();

    ~PolygonEdgeItem() override;

    protected:
    BaseEdgeConstraint *edgeConstraint = nullptr;

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    static void bresenham(QPainter *painter, const QPointF &start, const QPointF &end, qreal width);

    friend QDataStream &operator<<(QDataStream &out, const PolygonEdgeItem &edge);
    friend class PolygonItem;
};

#endif // POLYGONEDITOR_POLYGONEDGEITEM_H
