//
// Created by wookie on 10/14/24.
//

#include "Constraints/ConstraintChecker.h"
#include "Constraints/BaseEdgeConstraint.h"
#include "Edge/EdgeItemNormal.h"
#include "VertexItem.h"

bool ConstraintChecker::runCheck(EdgeItemNormal *edge, EdgeItemNormal *stopEdge, SearchDirection direction)
{
    if (edge == nullptr)
    {
        return true;
    }

    BaseEdgeConstraint *constraint = edge->getConstraint();
    if (constraint && !constraint->check(edge, direction))
    {
        return false;
    }

    return runCheckInternal(getNextEdge(edge, direction), stopEdge, direction);
}

void ConstraintChecker::runApply(EdgeItemNormal *edge, EdgeItemNormal *stopEdge, SearchDirection direction)
{
    if (edge == nullptr)
    {
        return;
    }

    BaseEdgeConstraint *constraint = edge->getConstraint();
    if (constraint)
    {
        constraint->apply(edge, direction);
    }

    runApplyInternal(getNextEdge(edge, direction), stopEdge, direction);
}

bool ConstraintChecker::runCheckInternal(EdgeItemNormal *edge, EdgeItemNormal *stopEdge, SearchDirection direction)
{
    if (edge == nullptr || edge == stopEdge)
    {
        return true;
    }

    BaseEdgeConstraint *constraint = edge->getConstraint();
    if (constraint && !constraint->check(edge, direction))
    {
        return false;
    }

    return runCheckInternal(getNextEdge(edge, direction), stopEdge, direction);
}

void ConstraintChecker::runApplyInternal(EdgeItemNormal *edge, EdgeItemNormal *stopEdge, SearchDirection direction)
{
    if (edge == nullptr || edge == stopEdge)
    {
        return;
    }

    BaseEdgeConstraint *constraint = edge->getConstraint();
    if (constraint)
    {
        constraint->apply(edge, direction);
    }

    runApplyInternal(getNextEdge(edge, direction), stopEdge, direction);
}

EdgeItemNormal *ConstraintChecker::getNextEdge(EdgeItemNormal *edge, SearchDirection direction)
{
    return direction == SearchDirection::Forward ? edge->getEndVertex()->edgeOut : edge->getStartVertex()->edgeIn;
}
