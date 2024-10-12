#ifndef POLYGONEDITOR_POLYGONITEM_H
#define POLYGONEDITOR_POLYGONITEM_H

#include "VertexItem.h"
#include <QGraphicsItem>
#include <QList>
#include <QPainter>
#include <gtest/gtest.h>

#include "EdgeItem.h"

class PolygonItem : public QGraphicsItem
{
    public:
    PolygonItem();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void appendVertex(const QPointF &position);

    void deleteVertex(unsigned int index);
    void deleteVertex(VertexItem *vertex);

    void subdivideEdge(unsigned int index);
    void subdivideEdge(EdgeItem *edge);

    ~PolygonItem();

    protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    private:
    void addEdge(EdgeItem *edge, unsigned int idx = -1);
    void deleteEdge(EdgeItem *edge);
    int findClosestVertex(const QPointF &pos);

    bool checkLinearOrdering();

    QList<VertexItem *> vertices;
    QList<EdgeItem *> edges;
    //    QHash<VertexItem *, QList<EdgeItem *>> vertexEdges;
    int selectedVertexIndex;

    FRIEND_TEST(PolygonItemTest, CheckLinearOrdering);
    void printOrderingStatus(VertexItem *vertex, const QString &message) const;
};

#endif // POLYGONEDITOR_POLYGONITEM_H
