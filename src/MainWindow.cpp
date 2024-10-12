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

    QAction *action1 = new QAction(QIcon("assets/icons/icon1.png"), "Action 1", this);
    QAction *action2 = new QAction(QIcon("assets/icons/icon2.png"), "Action 2", this);
    QAction *action3 = new QAction(QIcon("assets/icons/icon3.png"), "Action 3", this);

    topToolBar->addAction(action1);
    topToolBar->addAction(action2);
    topToolBar->addAction(action3);

    // Create the left custom toolbar (25% of the width or fixed 500px)
    QWidget *leftToolBar = new QWidget(this);
    leftToolBar->setMaximumWidth(350);

    QVBoxLayout *leftLayout = new QVBoxLayout(leftToolBar);
    QPushButton *button1    = new QPushButton("Button 1", leftToolBar);
    QPushButton *button2    = new QPushButton("Button 2", leftToolBar);
    QPushButton *button3    = new QPushButton("Button 3", leftToolBar);

    leftLayout->addWidget(button1);
    leftLayout->addWidget(button2);
    leftLayout->addWidget(button3);
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
