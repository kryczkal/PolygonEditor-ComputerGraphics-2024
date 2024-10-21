//
// Created by wookie on 10/14/24.
//

#ifndef POLYGONEDITOR_HORIZONTALEDGECONSTRAINT_H
#define POLYGONEDITOR_HORIZONTALEDGECONSTRAINT_H
#include "BaseEdgeConstraint.h"
enum class SearchDirection;

class HorizontalEdgeConstraint : public BaseEdgeConstraint
{
    public:
    bool check(EdgeItemNormal *edge, SearchDirection direction) override;
    void apply(EdgeItemNormal *edge, SearchDirection direction) override;
    void paintIcon(QPainter *painter, const QPointF &center, const EdgeItemNormal *) override;
};

#endif // POLYGONEDITOR_HORIZONTALEDGECONSTRAINT_H
