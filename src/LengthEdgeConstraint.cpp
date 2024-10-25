//
// Created by wookie on 10/14/24.
//

#include "Constraints/LengthEdgeConstraint.h"
#include "Constraints/ConstraintChecker.h"
#include "Edge/PolygonEdgeItem.h"
#include "Vertex/PolygonVertexItem.h"

bool LengthEdgeConstraint::check(PolygonEdgeItem *edge, SearchDirection direction)
{
    if (direction == SearchDirection::Forward)
    {
        return QLineF(edge->getStartVertex()->pos(), edge->getEndVertex()->pos()).length() == this->constraintLength;
    }
    else
    {
        return QLineF(edge->getEndVertex()->pos(), edge->getStartVertex()->pos()).length() == this->constraintLength;
    }
}

void LengthEdgeConstraint::apply(PolygonEdgeItem *edge, SearchDirection direction)
{
    if (direction == SearchDirection::Forward)
    {
        QLineF line(edge->getStartVertex()->pos(), edge->getEndVertex()->pos());
        line.setLength(this->constraintLength);
        edge->getEndVertex()->setPos(line.p2());
    }
    else
    {
        QLineF line(edge->getEndVertex()->pos(), edge->getStartVertex()->pos());
        line.setLength(this->constraintLength);
        edge->getStartVertex()->setPos(line.p2());
    }
}

void LengthEdgeConstraint::paintIcon(QPainter *painter, const QPointF &center, const PolygonEdgeItem *edge)
{
    painter->setPen(QColor(255, 0, 0));
    QFont font = painter->font();
    font.setPointSize(20);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(
        center, "L " + QString::number(QLineF(edge->getStartVertex()->pos(), edge->getEndVertex()->pos()).length())
    );
}
