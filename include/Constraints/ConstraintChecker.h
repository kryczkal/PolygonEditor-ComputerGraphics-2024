//
// Created by wookie on 10/14/24.
//

#ifndef POLYGONEDITOR_CONSTRAINTCHECKER_H
#define POLYGONEDITOR_CONSTRAINTCHECKER_H

class EdgeItemNormal;

enum class SearchDirection
{
    Forward,
    Backward
};
class ConstraintChecker
{
    public:
    ConstraintChecker() = delete;
    /*
     * Check if constraints are met for all edges starting from edge to stopEdge
     * @param edge - edge to start checking from
     * @param stopEdge - edge to stop checking at
     * @param direction - direction of the search
     * @return true if all constraints are met, false otherwise
     */
    static bool
    runCheck(EdgeItemNormal *edge, EdgeItemNormal *stopEdge, SearchDirection direction = SearchDirection::Forward);
    /*
     * Best effort to apply constraints to all edges starting from edge to stopEdge
     * @param edge - edge to start applying from
     * @param stopEdge - edge to stop applying at
     * @param direction - direction of the search and constraints application
     */
    static void
    runApply(EdgeItemNormal *edge, EdgeItemNormal *stopEdge, SearchDirection direction = SearchDirection::Forward);

    private:
    static bool runCheckInternal(EdgeItemNormal *edge, EdgeItemNormal *stopEdge, SearchDirection direction);
    static void runApplyInternal(EdgeItemNormal *edge, EdgeItemNormal *stopEdge, SearchDirection direction);
    static EdgeItemNormal *getNextEdge(EdgeItemNormal *edge, SearchDirection direction);
};

#endif // POLYGONEDITOR_CONSTRAINTCHECKER_H
