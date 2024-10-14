//
// Created by wookie on 10/14/24.
//

#ifndef POLYGONEDITOR_BASEEDGECONSTRAINT_H
#define POLYGONEDITOR_BASEEDGECONSTRAINT_H

class EdgeItem;
enum class SearchDirection;

class BaseEdgeConstraint
{
    public:
    virtual ~BaseEdgeConstraint()                                 = default;
    virtual bool check(EdgeItem *edge, SearchDirection direction) = 0;
    /*
     * @brief Applies the constraint to the edge
     * @param edge - edge to which the constraint is applied, if a correction is required, adjusting the in or out
     * vertex depending on the direction
     * @details The method modifies the edge according to the constraint
     * @param direction - direction of the search and correction
     * @return void
     */
    virtual void apply(EdgeItem *edge, SearchDirection direction) = 0;
};

#endif // POLYGONEDITOR_BASEEDGECONSTRAINT_H
