//
// Created by wookie on 10/21/24.
//

#ifndef POLYGONEDITOR_STANDALONEEDGEITEM_H
#define POLYGONEDITOR_STANDALONEEDGEITEM_H

#include "Edge/BaseEdgeItem.h"

class BaseVertexItem;

class StandaloneEdgeItem : public BaseEdgeItem
{
    public:
    StandaloneEdgeItem(BaseVertexItem *start, BaseVertexItem *end, QGraphicsItem *parent = nullptr);
    [[nodiscard]] QRectF boundingRect() const override;
    void paintIndex(QPainter *painter, QPointF startPoint, QPointF endPoint, int edgeIndex) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    [[nodiscard]] BaseEdgeConstraint *getConstraint() const override;

    protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

    friend QDataStream &operator<<(QDataStream &out, const StandaloneEdgeItem &edge);
    friend class PolygonItem;
};

#endif // POLYGONEDITOR_STANDALONEEDGEITEM_H
