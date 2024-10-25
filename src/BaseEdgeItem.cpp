//
// Created by wookie on 10/21/24.
//

#include "Edge/BaseEdgeItem.h"
#include "PolygonItem.h"
#include "Vertex/PolygonVertexItem.h"
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QInputDialog>
#include <QMenu>

BaseEdgeItem::BaseEdgeItem(BaseVertexItem *start, BaseVertexItem *end, QGraphicsItem *parent)
    : QGraphicsItem(parent), startVertex(start), endVertex(end)
{
    Q_ASSERT(start != nullptr);
    Q_ASSERT(end != nullptr);
    setZValue(0);
}

BaseVertexItem *BaseEdgeItem::getStartVertex() const { return startVertex; }
BaseVertexItem *BaseEdgeItem::getEndVertex() const { return endVertex; }

BaseEdgeItem::~BaseEdgeItem() {
}
