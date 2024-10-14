//
// Created by wookie on 10/14/24.
//
#include "Constraints/VerticalEdgeConstraint.h"
#include "Constraints/ConstraintChecker.h"
#include "EdgeItem.h"
#include "VertexItem.h"

bool VerticalEdgeConstraint::check(EdgeItem *edge, SearchDirection direction)
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

void VerticalEdgeConstraint::apply(EdgeItem *edge, SearchDirection direction)
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
