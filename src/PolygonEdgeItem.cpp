#include "Edge/PolygonEdgeItem.h"
#include "Constraints/ConstraintChecker.h"
#include "Constraints/HorizontalEdgeConstraint.h"
#include "Constraints/LengthEdgeConstraint.h"
#include "Constraints/VerticalEdgeConstraint.h"
#include "Edge/BaseEdgeItem.h"
#include "PolygonItem.h"
#include "Vertex/PolygonVertexItem.h"
#include <QAction>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QInputDialog>
#include <QMenu>
#include <QObject>
#include <QPainter>

PolygonEdgeItem::PolygonEdgeItem(PolygonVertexItem *start, PolygonVertexItem *end, QGraphicsItem *parent)
    : BaseEdgeItem(start, end, parent)
{
    Q_ASSERT(dynamic_cast<PolygonItem *>(parent) != nullptr); // Parent must be a PolygonItem
}
PolygonEdgeItem::PolygonEdgeItem(BaseVertexItem *start, BaseVertexItem *end, QGraphicsItem *parent) :
    BaseEdgeItem(start, end, parent) {
    PolygonVertexItem *startVertex = dynamic_cast<PolygonVertexItem *>(start);
    PolygonVertexItem *endVertex   = dynamic_cast<PolygonVertexItem *>(end);
    Q_ASSERT(startVertex != nullptr);
    Q_ASSERT(endVertex != nullptr);

    Q_ASSERT(dynamic_cast<PolygonItem *>(parent) != nullptr); // Parent must be a PolygonItem
}

QRectF PolygonEdgeItem::boundingRect() const
{
    QPointF startPoint = mapFromItem(startVertex, 0, 0);
    QPointF endPoint   = mapFromItem(endVertex, 0, 0);
    return QRectF(startPoint, endPoint).normalized().adjusted(-2, -2, 2, 2);
}

void PolygonEdgeItem::paintIndex(QPainter *painter, QPointF startPoint, QPointF endPoint, int edgeIndex)
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

void PolygonEdgeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
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

void PolygonEdgeItem::drawLine(QPainter *painter, QPointF startPoint, QPointF endPoint, bool useBresenham)
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

void PolygonEdgeItem::bresenham(QPainter *painter, const QPointF &start, const QPointF &end, qreal width)
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

QDataStream &operator<<(QDataStream &out, const PolygonEdgeItem &edge)
{
    out << "(" << edge.startVertex->pos() << ", " << edge.endVertex->pos() << ")";
    return out;
}

void PolygonEdgeItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
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
    QAction *changeToBezierAction;

    if (edgeConstraint)
    {
        removeConstraintAction = menu.addAction("Remove Constraint");
        QIODevice::connect(
            removeConstraintAction, &QAction::triggered,
            [this]
            {
                deleteConstraint();
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
                addHorizontalConstraint();
            }
        );
        setVerticalConstraintAction = menu.addAction("Set Vertical Constraint");
        QIODevice::connect(
            setVerticalConstraintAction, &QAction::triggered,
            [this]
            {
                addVerticalConstraint();
            }
        );
        setLengthConstraintAction = menu.addAction("Set Length Constraint");
        QIODevice::connect(
            setLengthConstraintAction, &QAction::triggered,
            [&]
            {
                addLengthConstraint();
            }
        );
    }

    changeToBezierAction = menu.addAction("Change to Bezier");
    QIODevice::connect(
        changeToBezierAction, &QAction::triggered,
        [this]
        {
            makeBezier();
        }
    );

    menu.exec(event->screenPos());
}


[[maybe_unused]] void PolygonEdgeItem::setConstraint(BaseEdgeConstraint *edgeConstraint)
{
    this->edgeConstraint = edgeConstraint;
}

void PolygonEdgeItem::deleteConstraint()
{
                delete edgeConstraint;
                edgeConstraint = nullptr;
}

PolygonEdgeItem::~PolygonEdgeItem() { delete edgeConstraint; }

BaseEdgeConstraint *PolygonEdgeItem::getConstraint() const {
    return this->edgeConstraint;
}

void PolygonEdgeItem::addLengthConstraint()
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

void PolygonEdgeItem::addLengthConstraintNoWindow()
{
    double length = QLineF(startVertex->pos(), endVertex->pos()).length();
                    edgeConstraint = new LengthEdgeConstraint(length);
                    dynamic_cast<PolygonItem *>(parentItem())->applyConstraints(this);
                    ConstraintChecker::runApply(this, this);
                    scene()->update();
}

void PolygonEdgeItem::addHorizontalConstraint()
{
                edgeConstraint = new HorizontalEdgeConstraint();
                dynamic_cast<PolygonItem *>(parentItem())->applyConstraints(this);
                scene()->update();
}

void PolygonEdgeItem::addVerticalConstraint()
{
                edgeConstraint = new VerticalEdgeConstraint();
                dynamic_cast<PolygonItem *>(parentItem())->applyConstraints(this);
                scene()->update();
}

void PolygonEdgeItem::makeBezier()
{
            PolygonItem *polygon = dynamic_cast<PolygonItem *>(parentItem());
            polygon->changeEdgeToBezier(this);
}

