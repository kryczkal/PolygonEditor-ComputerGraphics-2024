//
// Created by wookie on 10/14/24.
//

#include "Constraints/HorizontalEdgeConstraint.h"
#include "Constraints/ConstraintChecker.h"
#include "EdgeItem.h"
#include "VertexItem.h"

bool HorizontalEdgeConstraint::check(EdgeItem *edge, SearchDirection direction)
{
    if (direction == SearchDirection::Forward)
    {
        return edge->getStartVertex()->y() == edge->getEndVertex()->y();
    }
    else
    {
        return edge->getEndVertex()->y() == edge->getStartVertex()->y();
    }
}

void HorizontalEdgeConstraint::apply(EdgeItem *edge, SearchDirection direction)
{
    if (direction == SearchDirection::Forward)
    {
        edge->getEndVertex()->setY(edge->getStartVertex()->y());
    }
    else
    {
        edge->getStartVertex()->setY(edge->getEndVertex()->y());
    }
}

void HorizontalEdgeConstraint::paintIcon(QPainter *painter, const QPointF &center)
{
    painter->setPen(QColor(0, 255, 0));
    QFont font = painter->font();
    font.setPointSize(20);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(center, "H");
}