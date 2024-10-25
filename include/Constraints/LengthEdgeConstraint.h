//
// Created by wookie on 10/14/24.
//

#ifndef POLYGONEDITOR_LENGTHEDGECONSTRAINT_H
#define POLYGONEDITOR_LENGTHEDGECONSTRAINT_H

#include "BaseEdgeConstraint.h"
enum class SearchDirection;

class LengthEdgeConstraint : public BaseEdgeConstraint
{
    public:
    explicit LengthEdgeConstraint(double length) : constraintLength(length) {}
    bool check(PolygonEdgeItem *edge, SearchDirection direction) override;
    void apply(PolygonEdgeItem *edge, SearchDirection direction) override;
    void paintIcon(QPainter *painter, const QPointF &center, const PolygonEdgeItem *) override;

    private:
    double constraintLength;
};

#endif // POLYGONEDITOR_LENGTHEDGECONSTRAINT_H
