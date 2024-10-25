//
// Created by wookie on 10/14/24.
//

#ifndef POLYGONEDITOR_BASEEDGECONSTRAINT_H
#define POLYGONEDITOR_BASEEDGECONSTRAINT_H
#include <QPainter>

class PolygonEdgeItem;
enum class SearchDirection;

class BaseEdgeConstraint
{
    public:
    virtual ~BaseEdgeConstraint()                                       = default;
    virtual bool check(PolygonEdgeItem *edge, SearchDirection direction) = 0;
    /*
     * @brief Applies the edgeConstraint to the edge
     * @param edge - edge to which the edgeConstraint is applied, if a correction is required, adjusting the in or out
     * vertex depending on the direction
     * @details The method modifies the edge according to the edgeConstraint
     * @param direction - direction of the search and correction
     * @return void
     */
    virtual void apply(PolygonEdgeItem *edge, SearchDirection direction)                      = 0;
    virtual void paintIcon(QPainter *painter, const QPointF &center, const PolygonEdgeItem *) = 0;
};

#endif // POLYGONEDITOR_BASEEDGECONSTRAINT_H
