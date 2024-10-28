#ifndef POLYGONEDITOR_POLYGONITEM_H
#define POLYGONEDITOR_POLYGONITEM_H

#include "Edge/BezierEdgeItem.h"
#include "Vertex/PolygonVertexItem.h"
#include <QGraphicsItem>
#include <QList>
#include <QPainter>
#include <gtest/gtest.h>

class PolygonEdgeItem;
class BaseEdgeItem;

class PolygonItem : public QGraphicsItem
{
    public:
    PolygonItem();

    void createDefaultPolygon();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void appendVertex(const QPointF &position);
    void insertVertex(unsigned int index, const QPointF &position);

    void deleteVertex(unsigned int index);
    void deleteVertex(PolygonVertexItem *vertex);

    void subdivideEdge(unsigned int index);
    void subdivideEdge(BaseEdgeItem *edge);

    int getVertexIndex(PolygonVertexItem *vertex) const;
    int getEdgeIndex(BaseEdgeItem *edge) const;
    void toggleIndexVisibility();
    void toggleMoveAllVertices();

    void applyConstraints(BaseEdgeItem *edge);

    bool paintIndex      = true;
    bool moveAllVertices = false;
    bool useBresenham    = true;

    ~PolygonItem();

    void changeEdgeToBezier(PolygonEdgeItem *edgeItemNormal);

    void changeEdgeToNormal(BezierEdgeItem *edgeItemBezier);

    protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    private:
    void addEdge(BaseEdgeItem *edge, unsigned int idx = -1);
    void deleteEdge(BaseEdgeItem *edge);
    int findClosestVertex(const QPointF &pos);

    bool checkLinearOrdering();

    QList<BaseVertexItem *> vertices;
    QList<BaseEdgeItem *> edges;
    int selectedVertexIndex;

    FRIEND_TEST(PolygonItemTest, CheckLinearOrdering);
    void printOrderingStatus(BaseVertexItem *vertex, const QString &message) const;
};

#endif // POLYGONEDITOR_POLYGONITEM_H
