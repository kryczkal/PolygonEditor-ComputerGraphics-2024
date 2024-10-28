#include "Vertex/PolygonVertexItem.h"
#include "Constraints/C1VertexConstraint.h"
#include "Constraints/ConstraintChecker.h"
#include "Constraints/G1VertexConstraint.h"
#include "Edge/PolygonEdgeItem.h"
#include "PolygonItem.h"
#include "Vertex/BaseVertexItem.h"
#include <QAction>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>

static constexpr qreal radius = 5.0;

PolygonVertexItem::PolygonVertexItem(const QPointF &position, QGraphicsItem *parent) : BaseVertexItem(position, parent)
{
    Q_ASSERT(dynamic_cast<PolygonItem *>(parent) != nullptr); // Parent must be a PolygonItem
}

void PolygonVertexItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    BaseVertexItem::paint(painter, nullptr, nullptr);

    // Paint the index number of the vertex
    PolygonItem *polygon = dynamic_cast<PolygonItem *>(parentItem());
    if (polygon->paintIndex)
    {
        QString text         = QString::number(polygon->getVertexIndex(this));
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

void PolygonVertexItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction *deleteAction = menu.addAction("Delete");
    QIODevice::connect(
        deleteAction, &QAction::triggered,
        [this]()
        {
            PolygonItem *polygon = dynamic_cast<PolygonItem *>(parentItem());
            polygon->deleteVertex(this);
        }
    );
    if (dynamic_cast<BezierEdgeItem *>(edgeIn) || dynamic_cast<BezierEdgeItem *>(edgeOut))
    {
        if (!constraint)
        {
            QAction *addG1Constraint = menu.addAction("Add G1 constraint");
            QIODevice::connect(
                addG1Constraint, &QAction::triggered,
                [this]()
                {
                    this->constraint = new G1VertexConstraint();
                    this->constraint->apply(this);
                }
            );
            QAction *addC1Constraint = menu.addAction("Add C1 constraint");
            QIODevice::connect(
                addC1Constraint, &QAction::triggered,
                [this]()
                {
                    this->constraint = new C1VertexConstraint();
                    this->constraint->apply(this);
                }
            );
        }
        else
        {
            QAction *removeConstraint = menu.addAction("Remove constraint");
            QIODevice::connect(
                removeConstraint, &QAction::triggered,
                [this]()
                {
                    delete this->constraint;
                    this->constraint = nullptr;
                }
            );
        }
    }

    menu.exec(event->screenPos());
}

QDataStream &operator<<(QDataStream &out, const PolygonVertexItem &vertex)
{
    out << vertex.pos();
    return out;
}

void PolygonVertexItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    PolygonItem *polygon = dynamic_cast<PolygonItem *>(parentItem());
    if (polygon->moveAllVertices)
        return;

    BaseVertexItem::mousePressEvent(event);
}

void PolygonVertexItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    PolygonItem *polygon = dynamic_cast<PolygonItem *>(parentItem());
    if (polygon->moveAllVertices)
        return;

    polygon->applyConstraints(edgeOut);
    if (constraint)
        constraint->apply(this);

    BaseVertexItem::mouseMoveEvent(event);
}

void PolygonVertexItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    PolygonItem *polygon = dynamic_cast<PolygonItem *>(parentItem());
    if (polygon->moveAllVertices)
        return;

    BaseVertexItem::mouseReleaseEvent(event);
}

BaseVertexConstraint *PolygonVertexItem::getConstraint() const { return constraint; }
