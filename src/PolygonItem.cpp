#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <iostream>

#include "Constraints/ConstraintChecker.h"
#include "Edge/BezierEdgeItem.h"
#include "Edge/PolygonEdgeItem.h"
#include "PolygonItem.h"

PolygonItem::PolygonItem() : selectedVertexIndex(-1) {}

void PolygonItem::createDefaultPolygon()
{
    appendVertex(QPointF(10, 10));
    appendVertex(QPointF(200, 10));
    appendVertex(QPointF(200, 200));
    appendVertex(QPointF(150, 300));
    appendVertex(QPointF(10, 200));
    PolygonEdgeItem *casted_edge = reinterpret_cast<PolygonEdgeItem *>(edges[0]);
    casted_edge->addHorizontalConstraint();
    casted_edge = reinterpret_cast<PolygonEdgeItem *>(edges[1]);
    casted_edge->addVerticalConstraint();
    casted_edge = reinterpret_cast<PolygonEdgeItem *>(edges[2]);
    casted_edge->addLengthConstraintNoWindow();
    casted_edge = reinterpret_cast<PolygonEdgeItem *>(edges[4]);
    casted_edge->makeBezier();
}

////////////////////////////////////////////////////////////////////////////////
//                              QGraphicsItem Functions                       //
////////////////////////////////////////////////////////////////////////////////
QRectF PolygonItem::boundingRect() const
{
    if (vertices.isEmpty())
        return QRectF();

    qreal minX = vertices[0]->pos().x(), minY = vertices[0]->pos().y();
    qreal maxX = minX, maxY = minY;

    for (const auto &vertex : vertices)
    {
        QPointF pos = vertex->pos();
        minX        = std::min(minX, pos.x());
        minY        = std::min(minY, pos.y());
        maxX        = std::max(maxX, pos.x());
        maxY        = std::max(maxY, pos.y());
    }

    return QRectF(QPointF(minX, minY), QPointF(maxX, maxY)).adjusted(-40, -40, 40, 40);
}
void PolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *qStyleOptionGraphicsItem, QWidget *qWidget)
{
}

////////////////////////////////////////////////////////////////////////////////
//                              Vertex Functions                              //
////////////////////////////////////////////////////////////////////////////////
void PolygonItem::appendVertex(const QPointF &position)
{
    PolygonVertexItem *newVertex = new PolygonVertexItem(position, this);
    vertices.append(newVertex);

    if (vertices.size() > 3)
    {
        // Remove the closing edge (from the last vertex to the first one)
        BaseEdgeItem *lastEdge = edges.last();

        BaseVertexItem *firstVertex = vertices[0];
        BaseVertexItem *lastVertex  = vertices[vertices.size() - 2];

        Q_ASSERT(lastEdge->startVertex == lastVertex && lastEdge->endVertex == firstVertex);
        Q_ASSERT(firstVertex->edgeIn == lastEdge && lastVertex->edgeOut == lastEdge);
        Q_ASSERT(firstVertex->edgeOut != nullptr && firstVertex->edgeIn != nullptr);
        Q_ASSERT(lastVertex->edgeOut != nullptr && lastVertex->edgeIn != nullptr);

        deleteEdge(lastEdge);
    }

    if (vertices.size() > 1)
    {
        // Create an edge from the previous vertex to this one
        BaseVertexItem *previousVertex = vertices[vertices.size() - 2];
        PolygonEdgeItem *newEdge       = new PolygonEdgeItem(previousVertex, newVertex, this);
        addEdge(newEdge);
    }

    if (vertices.size() > 2)
    {
        // Add a closing edge (from the last vertex to the first one)
        BaseVertexItem *firstVertex = vertices[0];
        BaseVertexItem *lastVertex  = vertices[vertices.size() - 1];

        Q_ASSERT(firstVertex != lastVertex);

        Q_ASSERT(firstVertex->edgeIn == nullptr && lastVertex->edgeOut == nullptr);
        Q_ASSERT(firstVertex->edgeOut != nullptr && lastVertex->edgeIn != nullptr);

        PolygonEdgeItem *closingEdge = new PolygonEdgeItem(lastVertex, firstVertex, this);
        addEdge(closingEdge);
    }

    // Update boundingRect
    prepareGeometryChange();

    Q_ASSERT(checkLinearOrdering());
    printOrderingStatus(nullptr, "Vertex added successfully.");
}
/*
 * Insert a new vertex at the specified index with the given position. Connect the new vertex to the previous and next
 * vertices with new edges. The new vertex will be inserted at the specified index, moving all subsequent vertices
 */
void PolygonItem::insertVertex(unsigned int index, const QPointF &position)
{
    if (index >= vertices.size() || vertices.size() < 3)
    {
        appendVertex(position);
        return;
    }
    PolygonVertexItem *newVertex = new PolygonVertexItem(position, this);

    BaseVertexItem *prevVertex = vertices[index];
    BaseVertexItem *nextVertex = vertices[(index + 1) % vertices.size()];

    vertices.insert(index + 1, newVertex);
    deleteEdge(prevVertex->edgeOut);
    addEdge(
        new PolygonEdgeItem(
            reinterpret_cast<PolygonVertexItem *>(prevVertex), reinterpret_cast<PolygonVertexItem *>(newVertex), this
        ),
        index
    );
    addEdge(
        new PolygonEdgeItem(
            reinterpret_cast<PolygonVertexItem *>(newVertex), reinterpret_cast<PolygonVertexItem *>(nextVertex), this
        ),
        index + 1
    );

    prepareGeometryChange();
    Q_ASSERT(checkLinearOrdering());
}

void PolygonItem::deleteVertex(unsigned int index)
{
    if (index >= vertices.size())
        return;

    BaseVertexItem *vertex = vertices[index];

    if (vertices.size() == 2)
    {
        Q_ASSERT(vertex->hasOneEdge());

        if (vertex->edgeIn != nullptr)
            deleteEdge(vertex->edgeIn);
        else
            deleteEdge(vertex->edgeOut);
    }
    else if (vertices.size() == 3)
    {
        // If is a triangle, remove the vertex and the edges, and add a new edge between the remaining vertices ensuring
        // the correct order
        Q_ASSERT(vertex->hasBothEdges());

        BaseEdgeItem *prevEdge = vertex->edgeIn;
        BaseEdgeItem *nextEdge = vertex->edgeOut;
        for (auto e : edges)
        {
            deleteEdge(e);
        }
        int lowerIdx  = std::min(vertices.indexOf(prevEdge->startVertex), vertices.indexOf(nextEdge->endVertex));
        int higherIdx = std::max(vertices.indexOf(prevEdge->startVertex), vertices.indexOf(nextEdge->endVertex));
        addEdge(new PolygonEdgeItem(vertices[lowerIdx], vertices[higherIdx], this));
    }
    else if (vertices.size() > 3)
    {
        Q_ASSERT(vertex->hasBothEdges());

        BaseEdgeItem *prevEdge = vertex->edgeIn;
        BaseEdgeItem *nextEdge = vertex->edgeOut;

        PolygonEdgeItem *mergeEdge = new PolygonEdgeItem(prevEdge->startVertex, nextEdge->endVertex, this);

        int mergeIdx = edges.indexOf(prevEdge);
        deleteEdge(prevEdge);
        deleteEdge(nextEdge);
        addEdge(mergeEdge, mergeIdx);
    }

    vertices.removeOne(vertex);
    delete vertex;

    prepareGeometryChange();
    Q_ASSERT(checkLinearOrdering());
}

void PolygonItem::deleteVertex(PolygonVertexItem *vertex)
{
    int index = vertices.indexOf(vertex);
    if (index != -1)
    {
        deleteVertex(index);
    }
}
////////////////////////////////////////////////////////////////////////////////
//                              Edge Functions                                //
////////////////////////////////////////////////////////////////////////////////
void PolygonItem::subdivideEdge(BaseEdgeItem *edge)
{
    Q_ASSERT(edge->startVertex != edge->endVertex);
    Q_ASSERT(edges.contains(edge));
    Q_ASSERT(vertices.contains(edge->startVertex));
    Q_ASSERT(vertices.contains(edge->endVertex));
    Q_ASSERT(edge->startVertex->edgeOut == edge || edge->startVertex->edgeIn == edge);

    int edgeIndex = edges.indexOf(edge);
    insertVertex(edgeIndex, (edge->startVertex->pos() + edge->endVertex->pos()) / 2.0);

    prepareGeometryChange();
}

void PolygonItem::addEdge(BaseEdgeItem *edge, unsigned int idx)
{
    Q_ASSERT(edge->startVertex != edge->endVertex);

    if (idx == -1)
        edges.append(edge);
    else
        edges.insert(idx, edge);

    edge->startVertex->edgeOut = edge;
    edge->endVertex->edgeIn    = edge;
}
void PolygonItem::deleteEdge(BaseEdgeItem *edge)
{
    Q_ASSERT(edge->startVertex->edgeOut == edge || edge->startVertex->edgeIn == edge);

    edge->startVertex->edgeOut = nullptr;
    edge->endVertex->edgeIn    = nullptr;

    edges.removeOne(edge);
    delete edge;
}
////////////////////////////////////////////////////////////////////////////////
//                               Input Handling                               //
////////////////////////////////////////////////////////////////////////////////
void PolygonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // Move the entire polygon if the moveAllVertices flag is set
    if (moveAllVertices)
    {
        this->setFlag(QGraphicsItem::ItemIsMovable, true);
        return;
    }
}

void PolygonItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // Move the entire polygon if the moveAllVertices flag is set
    if (moveAllVertices)
    {
        QPointF delta = event->pos() - event->lastPos();
        this->moveBy(delta.x(), delta.y());
        return;
    }
}

void PolygonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    // Stop moving the entire polygon if the moveAllVertices flag is set
    if (moveAllVertices)
    {
        this->setFlag(QGraphicsItem::ItemIsMovable, false);
    }
}

////////////////////////////////////////////////////////////////////////////////
//                              Helper Functions                              //
////////////////////////////////////////////////////////////////////////////////
void PolygonItem::toggleIndexVisibility()
{
    paintIndex = !paintIndex;
    scene()->update();
}
void PolygonItem::toggleMoveAllVertices() { moveAllVertices = !moveAllVertices; }
int PolygonItem::getVertexIndex(PolygonVertexItem *vertex) const { return vertices.indexOf(vertex); }

int PolygonItem::getEdgeIndex(BaseEdgeItem *edge) const { return edges.indexOf(edge); }
int PolygonItem::findClosestVertex(const QPointF &pos)
{
    QList<QGraphicsView *> views = scene()->views();
    if (views.isEmpty())
        return -1;
    QGraphicsView *view = views.first();
    int viewWidth       = view->viewport()->width();
    int viewHeight      = view->viewport()->height();

    qreal tolerance = 0.10 * qMin(viewWidth, viewHeight);
    tolerance       = qMax(tolerance, 5.0);

    for (int i = 0; i < vertices.size(); ++i)
    {
        if (QLineF(pos, vertices[i]->pos()).length() < tolerance)
        {
            return i;
        }
    }
    return -1;
}
/*
 * Check if the vertices are in linear order with the edges
 * (v1 -> e1 (v1,v2) -> v2 -> e2 (v2,v3) -> v3 -> e3 (v3,v1) -> v1)
 */
bool PolygonItem::checkLinearOrdering()
{
    // Edge Case: A polygon with less than 3 vertices
    if (vertices.size() < 3)
    {
        if (vertices.size() == 2)
        {
            BaseVertexItem *vertex1 = vertices[0];
            BaseVertexItem *vertex2 = vertices[1];

            Q_ASSERT(edges.size() == 1);
            // Check if there is an edge connecting the two vertices in the correct order
            bool hasEdge = (edges[0]->startVertex == vertex1 && edges[0]->endVertex == vertex2);
            if (!hasEdge)
            {
                printOrderingStatus(vertex1, QString("No edge found between vertex 0 and vertex 1."));
            }
            return hasEdge;
        }

        if (vertices.size() == 1)
        {
            // A single vertex cannot have any edges, hence ordering does not apply
            printOrderingStatus(nullptr, QString("Only one vertex present, ordering not applicable."));
            return true;
        }

        // If there are no vertices, ordering does not apply
        printOrderingStatus(nullptr, QString("No vertices present, ordering not applicable."));
        return true;
    }

    // General Case: A polygon with 3 or more vertices
    for (int i = 0; i < vertices.size(); ++i)
    {
        BaseVertexItem *vertex = vertices[i];

        // Each vertex in a valid polygon should have exactly 2 edges
        if (!vertex->hasBothEdges())
        {
            printOrderingStatus(vertex, QString("Vertex %1 does not have exactly two edges.").arg(i));
            return false;
        }

        // Determine previous and next vertex indices, accounting for wrap-around
        int prevIndex = (i == 0) ? vertices.size() - 1 : i - 1;
        int nextIndex = (i + 1) % vertices.size();

        BaseVertexItem *prevVertex = vertices[prevIndex];
        BaseVertexItem *nextVertex = vertices[nextIndex];

        // Verify that edges are connecting to both previous and next vertices
        bool hasPrevEdge = vertex->edgeIn != nullptr && vertex->edgeIn->startVertex == prevVertex;
        bool hasNextEdge = vertex->edgeOut != nullptr && vertex->edgeOut->endVertex == nextVertex;

        BaseEdgeItem *prevEdge = vertex->edgeIn;
        BaseEdgeItem *nextEdge = vertex->edgeOut;

        // If either the previous or next edge is missing, the order is incorrect
        if (!hasPrevEdge || !hasNextEdge)
        {
            if (!hasPrevEdge)
            {
                printOrderingStatus(
                    vertex, QString("No edge found between vertex %1 and the previous vertex %2.").arg(i).arg(prevIndex)
                );
            }
            else
            {
                printOrderingStatus(
                    vertex, QString("No edge found between vertex %1 and the next vertex %2.").arg(i).arg(nextIndex)
                );
            }
            return false;
        }

        // Ensure the edges are in the expected order
        if (nextEdge != edges[i])
        {
            printOrderingStatus(
                vertex, QString("Edge order is incorrect at vertex %1: expected edge %2 but found edge %3.")
                            .arg(i)
                            .arg(i)
                            .arg(edges.indexOf(nextEdge))
            );
            return false;
        }

        if (prevEdge != edges[prevIndex])
        {
            printOrderingStatus(
                vertex, QString("Edge order is incorrect at vertex %1: expected edge %2 but found edge $3")
                            .arg(i)
                            .arg(prevIndex)
                            .arg(edges.indexOf(prevEdge))
            );
            return false;
        }
    }

    return true;
}

void PolygonItem::printOrderingStatus(BaseVertexItem *vertex, const QString &message) const
{
    qDebug() << "Ordering status:";

    for (int i = 0; i < vertices.size(); ++i)
    {
        BaseVertexItem *v = vertices[i];
        qDebug() << "Vertex" << i << ": Position:" << v->pos();

        if (v->edgeOut != nullptr)
        {
            Q_ASSERT(v->edgeOut->startVertex == vertices[i]);
            Q_ASSERT(v->edgeOut->endVertex == vertices[(i + 1) % vertices.size()]);
            Q_ASSERT(edges.contains(v->edgeOut));

            qDebug() << " EdgeFrom idx" << edges.indexOf(v->edgeOut) << "(" << vertices.indexOf(v->edgeOut->startVertex)
                     << "," << vertices.indexOf(v->edgeOut->endVertex) << ")" << "from pos()"
                     << v->edgeOut->startVertex->pos() << "to pos()" << v->edgeOut->endVertex->pos();
        }
        if (v->edgeIn != nullptr)
        {
            Q_ASSERT(v->edgeIn->startVertex == vertices[i - 1 < 0 ? vertices.size() - 1 : i - 1]);
            Q_ASSERT(v->edgeIn->endVertex == vertices[i]);
            Q_ASSERT(edges.contains(v->edgeIn));
            qDebug() << " EdgeTo   idx" << edges.indexOf(v->edgeIn) << "(" << vertices.indexOf(v->edgeIn->startVertex)
                     << "," << vertices.indexOf(v->edgeIn->endVertex) << ")" << "from pos()"
                     << v->edgeIn->startVertex->pos() << "to pos()" << v->edgeIn->endVertex->pos();
        }
    }

    if (vertex != nullptr)
    {
        qDebug() << "Issue detected at vertex with pos():" << vertex->pos() << "Message:" << message;
    }
}

////////////////////////////////////////////////////////////////////////////////
//                               Destructor                                   //
////////////////////////////////////////////////////////////////////////////////
PolygonItem::~PolygonItem()
{
    qDeleteAll(vertices);
    qDeleteAll(edges);
}

void PolygonItem::applyConstraints(BaseEdgeItem *edge)
{
    Q_ASSERT(edge != nullptr);

    ConstraintChecker::runApply(edge, edge->getStartVertex()->edgeIn, SearchDirection::Forward);
    ConstraintChecker::runApply(edge, edge->getEndVertex()->edgeOut, SearchDirection::Backward);
}

void PolygonItem::changeEdgeToBezier(PolygonEdgeItem *edgeItemNormal)
{
    Q_ASSERT(edgeItemNormal != nullptr);
    Q_ASSERT(edges.contains(edgeItemNormal));

    int edgeIndex = edges.indexOf(edgeItemNormal);
    BezierEdgeItem *bezierEdge =
        new BezierEdgeItem(edgeItemNormal->getStartVertex(), edgeItemNormal->getEndVertex(), this);
    deleteEdge(edgeItemNormal);
    addEdge(bezierEdge, edgeIndex);
}

void PolygonItem::changeEdgeToNormal(BezierEdgeItem *edgeItemBezier)
{
    Q_ASSERT(edgeItemBezier != nullptr);
    Q_ASSERT(edges.contains(edgeItemBezier));

    int edgeIndex = edges.indexOf(edgeItemBezier);
    PolygonEdgeItem *normalEdge =
        new PolygonEdgeItem(edgeItemBezier->getStartVertex(), edgeItemBezier->getEndVertex(), this);
    deleteEdge(edgeItemBezier);
    addEdge(normalEdge, edgeIndex);
}
