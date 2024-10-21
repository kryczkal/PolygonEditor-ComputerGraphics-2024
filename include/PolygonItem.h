#ifndef POLYGONEDITOR_POLYGONITEM_H
#define POLYGONEDITOR_POLYGONITEM_H

#include "VertexItem.h"
#include <QGraphicsItem>
#include <QList>
#include <QPainter>
#include <gtest/gtest.h>

class EdgeItemNormal;

class PolygonItem : public QGraphicsItem
{
    public:
    PolygonItem();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void appendVertex(const QPointF &position);
    void insertVertex(unsigned int index, const QPointF &position);

    void deleteVertex(unsigned int index);
    void deleteVertex(VertexItem *vertex);

    void subdivideEdge(unsigned int index);
    void subdivideEdge(EdgeItemNormal *edge);

    int getVertexIndex(VertexItem *vertex) const;
    int getEdgeIndex(EdgeItemNormal *edge) const;
    void toggleIndexVisibility();
    void toggleMoveAllVertices();

    void applyConstraints(EdgeItemNormal *edge);

    bool paintIndex      = true;
    bool moveAllVertices = false;
    bool useBresenham    = true;

    ~PolygonItem();

    protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    private:
    void addEdge(EdgeItemNormal *edge, unsigned int idx = -1);
    void deleteEdge(EdgeItemNormal *edge);
    int findClosestVertex(const QPointF &pos);

    bool checkLinearOrdering();

    QList<VertexItem *> vertices;
    QList<EdgeItemNormal *> edges;
    int selectedVertexIndex;

    FRIEND_TEST(PolygonItemTest, CheckLinearOrdering);
    void printOrderingStatus(VertexItem *vertex, const QString &message) const;
};

#endif // POLYGONEDITOR_POLYGONITEM_H
