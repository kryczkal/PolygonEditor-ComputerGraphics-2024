#include "VertexItem.h"
#include "Constraints/ConstraintChecker.h"
#include "EdgeItem.h"
#include "PolygonItem.h"
#include <QAction>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>

static constexpr qreal radius = 5.0;

VertexItem::VertexItem(const QPointF &position, QGraphicsItem *parent) : QGraphicsItem(parent)
{
    assert(dynamic_cast<PolygonItem *>(parent) != nullptr); // Parent must be a PolygonItem
    setPos(position);
    setZValue(1);
}

QRectF VertexItem::boundingRect() const { return QRectF(-radius, -radius, 2 * radius, 2 * radius); }

void VertexItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(Qt::black);
    painter->drawEllipse(QPointF(0, 0), radius, radius);

    // Paint the index number of the vertex
    PolygonItem *polygon = dynamic_cast<PolygonItem *>(parentItem());
    if (polygon->paintIndex)
    {
        QString text         = QString::number(polygon->getVertexIndex(const_cast<VertexItem *>(this)));
        QPointF textPosition = QPointF(radius, -radius);

        QPainterPath path;
        path.addText(textPosition, painter->font(), text);

        // Draw the text outline
        QPen pen(Qt::black, 3.0);
        painter->setPen(pen);
        painter->drawPath(path);

        // Fill the text with white color
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::white);
        painter->drawPath(path);
    }
}

void VertexItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction *deleteAction   = menu.addAction("Delete");
    QAction *selectedAction = menu.exec(event->screenPos());

    PolygonItem *polygon = dynamic_cast<PolygonItem *>(parentItem());
    if (selectedAction == deleteAction)
    {
        polygon->deleteVertex(this);
    }
}

QDataStream &operator<<(QDataStream &out, const VertexItem &vertex)
{
    out << vertex.pos();
    return out;
}

bool VertexItem::hasBothEdges() const { return edgeOut != nullptr && edgeIn != nullptr; }

bool VertexItem::hasOneEdge() const { return (!hasBothEdges() && (edgeOut != nullptr || edgeIn != nullptr)); }

void VertexItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    PolygonItem *polygon = dynamic_cast<PolygonItem *>(parentItem());
    if (polygon->moveAllVertices)
        return;

    setFlag(QGraphicsItem::ItemIsMovable, true);
}

void VertexItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    PolygonItem *polygon = dynamic_cast<PolygonItem *>(parentItem());
    if (polygon->moveAllVertices)
        return;

    QPointF delta = event->pos() - event->lastPos();
    moveBy(delta.x(), delta.y());

    bool flipDirection = false;
    bool canMove       = true;
    if (edgeIn && edgeIn->getConstraint())
        flipDirection = true;
    if (flipDirection && edgeOut && edgeOut->getConstraint())
        canMove = false;

    if (canMove && !flipDirection)
        ConstraintChecker::runApply(edgeOut, edgeOut);
    else if (canMove)
        ConstraintChecker::runApply(edgeIn, edgeIn, SearchDirection::Backward);
    else
    {
        polygon->moveBy(delta.x(), delta.y());
        moveBy(-delta.x(), -delta.y());
    }

    prepareGeometryChange();
    scene()->update();
}

void VertexItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    PolygonItem *polygon = dynamic_cast<PolygonItem *>(parentItem());
    if (polygon->moveAllVertices)
        return;
    setFlag(QGraphicsItem::ItemIsMovable, false);
}
