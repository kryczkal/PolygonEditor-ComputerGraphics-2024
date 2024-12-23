//
// Created by wookie on 10/14/24.
//

#ifndef POLYGONEDITOR_VERTICALEDGECONSTRAINT_H
#define POLYGONEDITOR_VERTICALEDGECONSTRAINT_H

#include "BaseEdgeConstraint.h"

class VerticalEdgeConstraint : public BaseEdgeConstraint
{
    public:
    bool check(EdgeItem *edge, SearchDirection direction) override;
    void apply(EdgeItem *edge, SearchDirection direction) override;
    void paintIcon(QPainter *painter, const QPointF &center) override;
};

#endif // POLYGONEDITOR_VERTICALEDGECONSTRAINT_H
