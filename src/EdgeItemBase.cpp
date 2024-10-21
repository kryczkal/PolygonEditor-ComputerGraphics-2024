//
// Created by wookie on 10/21/24.
//

#include "Edge/EdgeItemBase.h"
#include "PolygonItem.h"
#include "VertexItem.h"
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QInputDialog>
#include <QMenu>

EdgeItemBase::EdgeItemBase(VertexItem *start, VertexItem *end, QGraphicsItem *parent)
    : QGraphicsItem(parent), startVertex(start), endVertex(end)
{
    Q_ASSERT(dynamic_cast<PolygonItem *>(parent) != nullptr); // Parent must be a PolygonItem
    setZValue(0);
}

VertexItem *EdgeItemBase::getStartVertex() const { return startVertex; }

VertexItem *EdgeItemBase::getEndVertex() const { return endVertex; }

EdgeItemBase::~EdgeItemBase() {}
