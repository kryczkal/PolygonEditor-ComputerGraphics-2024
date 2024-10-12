#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <iostream>

#include "PolygonItem.h"

PolygonItem::PolygonItem() : selectedVertexIndex(-1) {}

////////////////////////////////////////////////////////////////////////////////
//                              QGraphicsItem Functions                       //
////////////////////////////////////////////////////////////////////////////////
QRectF PolygonItem::boundingRect() const
{
    if (vertices.isEmpty())
        return QRectF();

    qreal minX = vertices[0]->position.x(), minY = vertices[0]->position.y();
    qreal maxX = minX, maxY = minY;

    for (const auto &vertex : vertices)
    {
        QPointF pos = vertex->position;
        if (pos.x() < minX)
            minX = pos.x();
        if (pos.y() < minY)
            minY = pos.y();
        if (pos.x() > maxX)
            maxX = pos.x();
        if (pos.y() > maxY)
            maxY = pos.y();
    }

    return QRectF(QPointF(minX, minY), QPointF(maxX, maxY)).adjusted(-15, -15, 15, 15);
}

void PolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *qStyleOptionGraphicsItem, QWidget *qWidget)
{
    for (auto edge : edges)
    {
        edge->paint(painter, qStyleOptionGraphicsItem, qWidget);
    }

    for (auto vertex : vertices)
    {
        vertex->paint(painter, qStyleOptionGraphicsItem, qWidget);
    }
}

////////////////////////////////////////////////////////////////////////////////
//                              Vertex Functions                              //
////////////////////////////////////////////////////////////////////////////////
void PolygonItem::addVertex(const QPointF &position)
{
    VertexItem *newVertex = new VertexItem(position, this);
    vertices.append(newVertex);

    if (vertices.size() > 3)
    {
        // Remove the closing edge (from the last vertex to the first one)
        EdgeItem *lastEdge = edges.last();

        VertexItem *firstVertex = vertices[0];
        VertexItem *lastVertex  = vertices[vertices.size() - 2];

        assert(lastEdge->startVertex == lastVertex && lastEdge->endVertex == firstVertex);
        assert(firstVertex->edgeTo == lastEdge && lastVertex->edgeFrom == lastEdge);
        assert(firstVertex->edgeFrom != nullptr && firstVertex->edgeTo != nullptr);
        assert(lastVertex->edgeFrom != nullptr && lastVertex->edgeTo != nullptr);

        removeEdge(lastEdge);
    }

    if (vertices.size() > 1)
    {
        // Create an edge from the previous vertex to this one
        VertexItem *previousVertex = vertices[vertices.size() - 2];
        EdgeItem *newEdge          = new EdgeItem(previousVertex, newVertex, this);
        addEdge(newEdge);
    }

    if (vertices.size() > 2)
    {
        // Add a closing edge (from the last vertex to the first one)
        VertexItem *firstVertex = vertices[0];
        VertexItem *lastVertex  = vertices[vertices.size() - 1];

        assert(firstVertex != lastVertex);

        assert(firstVertex->edgeTo == nullptr && lastVertex->edgeFrom == nullptr);
        assert(firstVertex->edgeFrom != nullptr && lastVertex->edgeTo != nullptr);

        EdgeItem *closingEdge = new EdgeItem(lastVertex, firstVertex, this);
        addEdge(closingEdge);
    }

    // Update boundingRect
    prepareGeometryChange();

    assert(checkLinearOrdering());
    printOrderingStatus(nullptr, "Vertex added successfully.");
}

void PolygonItem::deleteVertex(unsigned int index)
{
    if (index >= vertices.size())
        return;

    VertexItem *vertex = vertices[index];

    assert(vertices.size() <= 1 || (vertex->edgeTo != nullptr || vertex->edgeFrom != nullptr));
    assert(vertices.size() <= 2 || (vertex->edgeTo != nullptr && vertex->edgeFrom != nullptr));

    if (vertices.size() == 2)
    {
        if (vertex->edgeTo != nullptr)
            removeEdge(vertex->edgeTo);
        else
            removeEdge(vertex->edgeFrom);
    }
    else if (vertices.size() > 2)
    {
        EdgeItem *prevEdge = vertex->edgeFrom;
        EdgeItem *nextEdge = vertex->edgeTo;

        EdgeItem *mergeEdge = new EdgeItem(prevEdge->startVertex, nextEdge->endVertex);

        int mergeIdx = edges.indexOf(prevEdge);
        removeEdge(prevEdge);
        removeEdge(nextEdge);
        addEdge(mergeEdge, mergeIdx);
    }

    vertices.removeOne(vertex);
    delete vertex;

    prepareGeometryChange();
    scene()->update();

    assert(checkLinearOrdering());
}

void PolygonItem::deleteVertex(VertexItem *vertex)
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
void PolygonItem::subdivideEdge(EdgeItem *edge)
{
    //        VertexItem *start = edge->startVertex;
    //        VertexItem *end = edge->endVertex;
    //
    //        QPointF midPoint = (start->position + end->position) / 2;
    //        addVertex(midPoint);
    //
    //        // Find the index of the new vertex
    //        int index = vertices.size() - 1;
    //
    //        // Find the index of the edge
    //        int edgeIndex = edges.indexOf(edge);
    //
    //        // Remove the edge
    //        edges.removeAt(edgeIndex);
    //        delete edge;
    //
    //        // Create two new edges
    //        EdgeItem *firstEdge = new EdgeItem(start, vertices[index], this);
    //        EdgeItem *secondEdge = new EdgeItem(vertices[index], end, this);
    //
    //        edges.insert(edgeIndex, firstEdge);
    //        edges.insert(edgeIndex + 1, secondEdge);
    //
    //        vertexEdges[start].removeOne(edge);
    //        vertexEdges[end].removeOne(edge);
    //
    //        vertexEdges[start].append(firstEdge);
    //        vertexEdges[vertices[index]].append(firstEdge);
    //
    //        vertexEdges[vertices[index]].append(secondEdge);
    //        vertexEdges[end].append(secondEdge);
    //
    //        prepareGeometryChange();
    //        scene()->update();
}

void PolygonItem::addEdge(EdgeItem *edge, int idx)
{
    assert(edge->startVertex != edge->endVertex);

    if (idx == -1)
        edges.append(edge);
    else
        edges.insert(idx, edge);

    edge->startVertex->edgeFrom = edge;
    edge->endVertex->edgeTo      = edge;
}
void PolygonItem::removeEdge(EdgeItem *edge)
{
    assert( !(edge->startVertex->edgeFrom != nullptr && edge->startVertex->edgeTo != nullptr) );
    assert( !(edge->endVertex->edgeFrom != nullptr && edge->endVertex->edgeTo != nullptr) );
    assert (edge->startVertex->edgeFrom == edge || edge->startVertex->edgeTo == edge);

    edge->startVertex->edgeFrom = nullptr;
    edge->endVertex->edgeTo      = nullptr;

    edges.removeOne(edge);
    delete edge;
}
////////////////////////////////////////////////////////////////////////////////
//                               Input Handling                               //
////////////////////////////////////////////////////////////////////////////////
void PolygonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF clickPos    = event->pos();
    selectedVertexIndex = findClosestVertex(clickPos);
    if (selectedVertexIndex != -1)
    {
        vertices[selectedVertexIndex]->setFlag(QGraphicsItem::ItemIsMovable, true);
    }
}

void PolygonItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (selectedVertexIndex != -1)
    {
        vertices[selectedVertexIndex]->position = event->pos();
        scene()->views().first()->centerOn(vertices[selectedVertexIndex]);
        scene()->update();
    }
}

void PolygonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    if (selectedVertexIndex != -1)
    {
        vertices[selectedVertexIndex]->setFlag(QGraphicsItem::ItemIsMovable, false);
        scene()->views().first()->centerOn(vertices[selectedVertexIndex]);
    }
    selectedVertexIndex = -1;
}

////////////////////////////////////////////////////////////////////////////////
//                              Helper Functions                              //
////////////////////////////////////////////////////////////////////////////////
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
        if (QLineF(pos, vertices[i]->position).length() < tolerance)
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
            VertexItem *vertex1 = vertices[0];
            VertexItem *vertex2 = vertices[1];

            // Check if there is an edge connecting the two vertices in either direction
            bool hasEdge = false;
            for (EdgeItem *edge : vertexEdges[vertex1])
            {
                if ((edge->startVertex == vertex1 && edge->endVertex == vertex2) ||
                    (edge->startVertex == vertex2 && edge->endVertex == vertex1))
                {
                    hasEdge = true;
                    break;
                }
            }
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
        VertexItem *vertex = vertices[i];

        // Each vertex in a valid polygon should have exactly 2 edges
        if (!vertex->hasBothEdges())
        {
            printOrderingStatus(vertex, QString("Vertex %1 does not have exactly two edges.").arg(i));
            return false;
        }

        // Determine previous and next vertex indices, accounting for wrap-around
        int prevIndex = (i == 0) ? vertices.size() - 1 : i - 1;
        int nextIndex = (i + 1) % vertices.size();

        VertexItem *prevVertex = vertices[prevIndex];
        VertexItem *nextVertex = vertices[nextIndex];

        // Verify that edges are connecting to both previous and next vertices
        bool hasPrevEdge = false;
        bool hasNextEdge = false;

        EdgeItem *prevEdge = nullptr;
        EdgeItem *nextEdge = nullptr;

        for (EdgeItem *edge : vertexEdges[vertex])
        {
            if ((edge->startVertex == vertex && edge->endVertex == prevVertex) ||
                (edge->startVertex == prevVertex && edge->endVertex == vertex))
            {
                hasPrevEdge = true;
                prevEdge    = edge;
            }

            if ((edge->startVertex == vertex && edge->endVertex == nextVertex) ||
                (edge->startVertex == nextVertex && edge->endVertex == vertex))
            {
                hasNextEdge = true;
                nextEdge    = edge;
            }
        }

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

void PolygonItem::printOrderingStatus(VertexItem *vertex, const QString &message) const
{
    qDebug() << "Ordering status:";

    for (int i = 0; i < vertices.size(); ++i)
    {
        VertexItem *v = vertices[i];
        qDebug() << "Vertex" << i << ": Position:" << v->position;

        for (EdgeItem *edge : vertexEdges[v])
        {
            qDebug() << " Edge" << edges.indexOf(edge) << "(" << vertices.indexOf(edge->startVertex) << ","
                     << vertices.indexOf(edge->endVertex) << ")" << "from position" << edge->startVertex->position
                     << "to position" << edge->endVertex->position;
        }
    }

    if (vertex != nullptr)
    {
        qDebug() << "Issue detected at vertex with position:" << vertex->position << "Message:" << message;
    }
}

////////////////////////////////////////////////////////////////////////////////
//                               Destructor                                   //
////////////////////////////////////////////////////////////////////////////////
PolygonItem::~PolygonItem()
{
    for (auto vertex : vertices)
    {
        delete vertex;
    }

    for (auto edge : edges)
    {
        delete edge;
    }

    vertices.clear();
    edges.clear();
}
