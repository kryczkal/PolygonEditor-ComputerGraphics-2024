#ifndef POLYGONEDITOR_POLYGONITEM_H
#define POLYGONEDITOR_POLYGONITEM_H

#include "EdgeItem.h"
#include "VertexItem.h"
#include <QGraphicsItem>
#include <QList>
#include <QPainter>

class PolygonItem : public QGraphicsItem
{
    public:
    PolygonItem();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void addVertex(const QPointF &position);

    void deleteVertex(unsigned int index);
    void deleteVertex(VertexItem *vertex);

    ~PolygonItem();

    protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    private:
    int findClosestVertex(const QPointF &pos);

    QList<VertexItem *> vertices;
    QList<EdgeItem *> edges;
    QHash<VertexItem *, QList<EdgeItem *>> vertexEdges;
    int selectedVertexIndex;
};

#endif // POLYGONEDITOR_POLYGONITEM_H
