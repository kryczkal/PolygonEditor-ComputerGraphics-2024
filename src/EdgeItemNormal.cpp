#include "Edge/EdgeItemNormal.h"
#include "Constraints/ConstraintChecker.h"
#include "Constraints/HorizontalEdgeConstraint.h"
#include "Constraints/LengthEdgeConstraint.h"
#include "Constraints/VerticalEdgeConstraint.h"
#include "Edge/EdgeItemBase.h"
#include "PolygonItem.h"
#include "VertexItem.h"
#include <QAction>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QInputDialog>
#include <QMenu>
#include <QObject>
#include <QPainter>

EdgeItemNormal::EdgeItemNormal(VertexItem *start, VertexItem *end, QGraphicsItem *parent)
    : EdgeItemBase(start, end, parent)
{
}

QRectF EdgeItemNormal::boundingRect() const
{
    QPointF startPoint = mapFromItem(startVertex, 0, 0);
    QPointF endPoint   = mapFromItem(endVertex, 0, 0);
    return QRectF(startPoint, endPoint).normalized().adjusted(-2, -2, 2, 2);
}

void EdgeItemNormal::paintIndex(QPainter *painter, QPointF startPoint, QPointF endPoint, int edgeIndex)
{
    QPointF center = (startPoint + endPoint) / 2;
    QString text   = QString::number(edgeIndex);

    QPainterPath path;
    path.addText(center, painter->font(), text);

    QPen pen(Qt::black, 3.0);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::white);
    painter->drawPath(path);
}

void EdgeItemNormal::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPointF startPoint = mapFromItem(startVertex, 0, 0);
    QPointF endPoint   = mapFromItem(endVertex, 0, 0);

    PolygonItem *polygon = dynamic_cast<PolygonItem *>(parentItem());
    drawLine(painter, startPoint, endPoint, polygon->useBresenham);
    if (polygon->paintIndex)
        paintIndex(painter, startPoint, endPoint, polygon->getEdgeIndex(this));
    if (edgeConstraint)
        edgeConstraint->paintIcon(painter, (startPoint + endPoint) / 2, this);
}

void EdgeItemNormal::drawLine(QPainter *painter, QPointF startPoint, QPointF endPoint, bool useBresenham)
{
    if (useBresenham)
    {
        painter->setRenderHint(QPainter::Antialiasing, false);
        painter->setPen(QPen(Qt::black, 1));
        bresenham(painter, startPoint, endPoint, width);
    }
    else
    {
        painter->setPen(QPen(Qt::black, width));
        painter->drawLine(startPoint, endPoint);
    }
}

void EdgeItemNormal::bresenham(QPainter *painter, const QPointF &start, const QPointF &end, qreal width)
{
    int x1 = static_cast<int>(start.x());
    int y1 = static_cast<int>(start.y());
    int x2 = static_cast<int>(end.x());
    int y2 = static_cast<int>(end.y());

    int dx  = std::abs(x2 - x1);
    int dy  = std::abs(y2 - y1);
    int sx  = (x1 < x2) ? 1 : -1;
    int sy  = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    int halfWidth = std::max(1, static_cast<int>(width / 2));

    while (true)
    {
        for (int i = -halfWidth; i <= halfWidth; ++i)
        {
            if (dx > dy)
            {
                painter->drawPoint(x1, y1 + i);
            }
            else
            {
                painter->drawPoint(x1 + i, y1);
            }
        }

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

QDataStream &operator<<(QDataStream &out, const EdgeItemNormal &edge)
{
    out << "(" << edge.startVertex->pos() << ", " << edge.endVertex->pos() << ")";
    return out;
}

void EdgeItemNormal::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction *subdivideAction = menu.addAction("Subdivide");
    QIODevice::connect(
        subdivideAction, &QAction::triggered,
        [this]
        {
            PolygonItem *polygon = dynamic_cast<PolygonItem *>(parentItem());
            polygon->subdivideEdge(this);
        }
    );

    QAction *setHorizontalConstraintAction;
    QAction *setVerticalConstraintAction;
    QAction *setLengthConstraintAction;
    QAction *removeConstraintAction;

    if (edgeConstraint)
    {
        removeConstraintAction = menu.addAction("Remove Constraint");
        QIODevice::connect(
            removeConstraintAction, &QAction::triggered,
            [this]
            {
                delete edgeConstraint;
                edgeConstraint = nullptr;
            }
        );
    }
    else
    {
        setHorizontalConstraintAction = menu.addAction("Set Horizontal Constraint");
        QIODevice::connect(
            setHorizontalConstraintAction, &QAction::triggered,
            [this]
            {
                edgeConstraint = new HorizontalEdgeConstraint();
                dynamic_cast<PolygonItem *>(parentItem())->applyConstraints(this);
                scene()->update();
            }
        );
        setVerticalConstraintAction = menu.addAction("Set Vertical Constraint");
        QIODevice::connect(
            setVerticalConstraintAction, &QAction::triggered,
            [this]
            {
                edgeConstraint = new VerticalEdgeConstraint();
                dynamic_cast<PolygonItem *>(parentItem())->applyConstraints(this);
                scene()->update();
            }
        );
        setLengthConstraintAction = menu.addAction("Set Length Constraint");
        QIODevice::connect(
            setLengthConstraintAction, &QAction::triggered,
            [&]
            {
                QInputDialog dialog;
                dialog.setLabelText("Length:");
                dialog.setDoubleMinimum(0);
                dialog.setDoubleMaximum(std::numeric_limits<double>::max());
                dialog.setDoubleValue(QLineF(startVertex->pos(), endVertex->pos()).length());
                QIODevice::connect(
                    &dialog, &QInputDialog::accepted,
                    [&]()
                    {
                        double length = dialog.doubleValue();
                        if (length <= 0)
                            return;
                        edgeConstraint = new LengthEdgeConstraint(length);
                        dynamic_cast<PolygonItem *>(parentItem())->applyConstraints(this);
                        ConstraintChecker::runApply(this, this);
                        scene()->update();
                    }
                );
                dialog.exec();
            }
        );
    }

    menu.exec(event->screenPos());
}

BaseEdgeConstraint *EdgeItemNormal::getConstraint() const { return edgeConstraint; }

[[maybe_unused]] void EdgeItemNormal::setConstraint(BaseEdgeConstraint *edgeConstraint)
{
    EdgeItemNormal::edgeConstraint = edgeConstraint;
}

EdgeItemNormal::~EdgeItemNormal() { delete edgeConstraint; }
