//
// Created by wookie on 10/14/24.
//
#include "Constraints/VerticalEdgeConstraint.h"
#include "Constraints/ConstraintChecker.h"
#include "Edge/PolygonEdgeItem.h"
#include "Vertex/PolygonVertexItem.h"

bool VerticalEdgeConstraint::check(PolygonEdgeItem *edge, SearchDirection direction)
{
    if (direction == SearchDirection::Forward)
    {
        return edge->getStartVertex()->x() == edge->getEndVertex()->x();
    }
    else
    {
        return edge->getEndVertex()->x() == edge->getStartVertex()->x();
    }
}

void VerticalEdgeConstraint::apply(PolygonEdgeItem *edge, SearchDirection direction)
{
    if (direction == SearchDirection::Forward)
    {
        edge->getEndVertex()->setX(edge->getStartVertex()->x());
    }
    else
    {
        edge->getStartVertex()->setX(edge->getEndVertex()->x());
    }
}

void VerticalEdgeConstraint::paintIcon(QPainter *painter, const QPointF &center, const PolygonEdgeItem *)
{
    painter->setPen(QColor(0, 0, 255));
    QFont font = painter->font();
    font.setPointSize(20);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(center, "V");
}
