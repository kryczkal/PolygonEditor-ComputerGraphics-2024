#ifndef POLYGONITEM_H
#define POLYGONITEM_H
#include <QGraphicsItem>
#include <QPainter>

class PolygonItem : public QGraphicsItem
{
public:
    PolygonItem(const QPolygonF &polygon);
    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setPolygon(const QPolygonF &newPolygon);
    QPolygonF getPolygon() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPolygonF polygon;
    int selectedVertexIndex;

    void drawBresenhamLine(QPainter *painter, const QPointF &start, const QPointF &end);
    int findClosestVertex(const QPointF &pos);
};

#endif // POLYGONITEM_H
