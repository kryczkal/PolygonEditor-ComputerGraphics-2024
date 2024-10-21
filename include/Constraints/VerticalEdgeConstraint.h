//
// Created by wookie on 10/14/24.
//

#ifndef POLYGONEDITOR_VERTICALEDGECONSTRAINT_H
#define POLYGONEDITOR_VERTICALEDGECONSTRAINT_H

#include "BaseEdgeConstraint.h"

class VerticalEdgeConstraint : public BaseEdgeConstraint
{
    public:
    bool check(EdgeItemNormal *edge, SearchDirection direction) override;
    void apply(EdgeItemNormal *edge, SearchDirection direction) override;
    void paintIcon(QPainter *painter, const QPointF &center, const EdgeItemNormal *) override;
};

#endif // POLYGONEDITOR_VERTICALEDGECONSTRAINT_H
