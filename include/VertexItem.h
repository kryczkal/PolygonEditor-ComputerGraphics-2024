//
// Created by wookie on 10/11/24.
//

#ifndef POLYGONEDITOR_VERTEXITEM_H
#define POLYGONEDITOR_VERTEXITEM_H

#include <QGraphicsItem>
#include <QList>
#include <QPainter>

class VertexItem : public QGraphicsItem
{
    public:
    VertexItem(const QPointF &position, QGraphicsItem *parent = nullptr);

    [[nodiscard]] QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setPosition(const QPointF &newPosition);
    QPointF getPosition() const;

    private:
    QPointF position;
    qreal radius = 5.0;
};

#endif // POLYGONEDITOR_VERTEXITEM_H
