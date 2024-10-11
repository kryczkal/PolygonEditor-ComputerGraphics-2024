#include "PolygonItem.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <cmath>

PolygonItem::PolygonItem() : selectedVertexIndex(-1) {}

QRectF PolygonItem::boundingRect() const
{
    if (vertices.isEmpty())
        return QRectF();

    qreal minX = vertices[0]->getPosition().x(), minY = vertices[0]->getPosition().y();
    qreal maxX = minX, maxY = minY;

    for (const auto &vertex : vertices)
    {
        QPointF pos = vertex->getPosition();
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

void PolygonItem::addVertex(const QPointF &position)
{
    VertexItem *newVertex = new VertexItem(position, this);
    vertices.append(newVertex);

    if (vertices.size() > 3)
    {
        // Remove the closing edge (from the last vertex to the first one)
        EdgeItem *lastEdge = edges.last();
        edges.removeLast();
        delete lastEdge;
    }

    if (vertices.size() > 1)
    {
        // Create an edge from the previous vertex to this one
        VertexItem *previousVertex = vertices[vertices.size() - 2];
        EdgeItem *newEdge          = new EdgeItem(previousVertex, newVertex, this);
        edges.append(newEdge);
    }

    if (vertices.size() > 2)
    {
        // Add a closing edge (from the last vertex to the first one)
        VertexItem *firstVertex = vertices[0];
        VertexItem *lastVertex  = vertices[vertices.size() - 1];
        EdgeItem *newEdge       = new EdgeItem(lastVertex, firstVertex, this);
        edges.append(newEdge);
    }

    // Update boundingRect
    prepareGeometryChange();
}

void PolygonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF clickPos    = event->pos();
    selectedVertexIndex = findClosestVertex(clickPos);
    if (selectedVertexIndex != -1)
    {
        setFlag(QGraphicsItem::ItemIsMovable, true);
        //        vertices[selectedVertexIndex]->setFlag(QGraphicsItem::ItemIsMovable, true);
    }
}

void PolygonItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (selectedVertexIndex != -1)
    {
        vertices[selectedVertexIndex]->setPosition(event->pos());
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
        if (QLineF(pos, vertices[i]->getPosition()).length() < tolerance)
        {
            return i;
        }
    }
    return -1;
}
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
}
