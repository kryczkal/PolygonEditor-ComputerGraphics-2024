//
// Created by wookie on 10/14/24.
//

#include "Constraints/ConstraintChecker.h"
#include "Constraints/BaseEdgeConstraint.h"
#include "Edge/BaseEdgeItem.h"
#include "Edge/BezierEdgeItem.h"
#include "Vertex/PolygonVertexItem.h"

bool ConstraintChecker::runCheck(BaseEdgeItem *edge, BaseEdgeItem *stopEdge, SearchDirection direction)
{
    if (edge == nullptr)
    {
        return true;
    }

    BaseEdgeConstraint *constraint = edge->getConstraint();
    if (constraint && !constraint->check(reinterpret_cast<PolygonEdgeItem *>(edge), direction))
    {
        return false;
    }

    return runCheckInternal(getNextEdge(edge, direction), stopEdge, direction);
}

void ConstraintChecker::runApply(BaseEdgeItem *edge, BaseEdgeItem *stopEdge, SearchDirection direction)
{
    if (edge == nullptr)
    {
        return;
    }

    BaseEdgeConstraint *constraint = edge->getConstraint();
    if (constraint)
    {
        constraint->apply(reinterpret_cast<PolygonEdgeItem *>(edge), direction);
    }
    BezierEdgeItem *castEdge = dynamic_cast<BezierEdgeItem *>(edge);
    if (castEdge)
    {
        PolygonVertexItem *startVertex = reinterpret_cast<PolygonVertexItem *>(castEdge->getStartVertex());
        PolygonVertexItem *endVertex   = reinterpret_cast<PolygonVertexItem *>(castEdge->getEndVertex());

        if (startVertex->getConstraint())
            startVertex->getConstraint()->apply(startVertex);
        if (endVertex->getConstraint())
            endVertex->getConstraint()->apply(endVertex);
    }

    runApplyInternal(getNextEdge(edge, direction), stopEdge, direction);
}

bool ConstraintChecker::runCheckInternal(BaseEdgeItem *edge, BaseEdgeItem *stopEdge, SearchDirection direction)
{
    if (edge == nullptr || edge == stopEdge)
    {
        return true;
    }

    BaseEdgeConstraint *constraint = edge->getConstraint();
    if (constraint && !constraint->check(reinterpret_cast<PolygonEdgeItem *>(edge), direction))
    {
        return false;
    }

    return runCheckInternal(getNextEdge(edge, direction), stopEdge, direction);
}

void ConstraintChecker::runApplyInternal(BaseEdgeItem *edge, BaseEdgeItem *stopEdge, SearchDirection direction)
{
    if (edge == nullptr || edge == stopEdge)
    {
        return;
    }

    BaseEdgeConstraint *constraint = edge->getConstraint();
    if (constraint)
    {
        constraint->apply(reinterpret_cast<PolygonEdgeItem *>(edge), direction);
    }
    BezierEdgeItem *castEdge = dynamic_cast<BezierEdgeItem *>(edge);
    if (castEdge)
    {
        PolygonVertexItem *startVertex = reinterpret_cast<PolygonVertexItem *>(castEdge->getStartVertex());
        PolygonVertexItem *endVertex   = reinterpret_cast<PolygonVertexItem *>(castEdge->getEndVertex());

        if (startVertex->getConstraint())
            startVertex->getConstraint()->apply(startVertex);
        if (endVertex->getConstraint())
            endVertex->getConstraint()->apply(endVertex);
    }

    runApplyInternal(getNextEdge(edge, direction), stopEdge, direction);
}

BaseEdgeItem *ConstraintChecker::getNextEdge(BaseEdgeItem *edge, SearchDirection direction)
{
    return direction == SearchDirection::Forward ? edge->getEndVertex()->edgeOut : edge->getStartVertex()->edgeIn;
}
