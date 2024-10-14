#include <QGraphicsView>
#include <QIcon>
#include <QPushButton>
#include <QSplitter>
#include <QToolBar>
#include <QVBoxLayout>

#include "MainWindow.h"
#include "PolygonItem.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // Create central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Create the top toolbar with icon buttons
    QToolBar *topToolBar = new QToolBar("Top Toolbar", this);
    addToolBar(Qt::TopToolBarArea, topToolBar);

    // Create the left custom toolbar (25% of the width or fixed 500px)
    QWidget *leftToolBar = new QWidget(this);
    leftToolBar->setMaximumWidth(350);

    QVBoxLayout *leftLayout = new QVBoxLayout(leftToolBar);
    QPushButton *button1    = new QPushButton("Delete Polygon", leftToolBar);
    QPushButton *button2    = new QPushButton("Add Vertex", leftToolBar);
    QPushButton *button3    = new QPushButton("Toggle index visibility", leftToolBar);
    QPushButton *button4    = new QPushButton("Toggle move all", leftToolBar);

    leftLayout->addWidget(button1);
    leftLayout->addWidget(button2);
    leftLayout->addWidget(button3);
    leftLayout->addWidget(button4);
    leftLayout->addStretch();

    // Create canvas to the right using QGraphicsView
    QGraphicsView *canvasView = new QGraphicsView(this);
    canvasView->setRenderHint(QPainter::Antialiasing);

    // Create a QGraphicsScene and add PolygonItem
    scene       = new QGraphicsScene(this);
    polygonItem = new PolygonItem();
    scene->addItem(polygonItem);
    canvasView->setScene(scene);

    // Create a splitter to split the left toolbar and the canvas
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, centralWidget);
    mainSplitter->addWidget(leftToolBar);
    mainSplitter->addWidget(canvasView);
    mainSplitter->setStretchFactor(0, 1);
    mainSplitter->setStretchFactor(1, 3);

    // Set the layout for the central widget
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(mainSplitter);

    // Connect button to the handler
    connect(button1, &QPushButton::clicked, this, &MainWindow::DeletePolygon);
    connect(button2, &QPushButton::clicked, this, &MainWindow::AddVertex);
    connect(button3, &QPushButton::clicked, this, &MainWindow::ToggleIndexVisibility);
    connect(button4, &QPushButton::clicked, this, &MainWindow::ToggleMoveAllVertices);
}

MainWindow::~MainWindow() {}

void MainWindow::DeletePolygon()
{
    delete polygonItem;
    polygonItem = new PolygonItem();
    scene->addItem(polygonItem);
    scene->update();
}
void MainWindow::AddVertex()
{
    QPointF position = QPointF(rand() % 500, rand() % 500);
    polygonItem->appendVertex(position);
    scene->update();
}

void MainWindow::ToggleIndexVisibility()
{
    polygonItem->toggleIndexVisibility();
    scene->update();
}

void MainWindow::ToggleMoveAllVertices()
{
    polygonItem->toggleMoveAllVertices();
    scene->update();
}