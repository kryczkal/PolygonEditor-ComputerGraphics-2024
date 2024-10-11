#include "mainwindow.h"
#include "PolygonItem.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);

    QPolygonF polygon;
    polygon << QPointF(50, 50) << QPointF(150, 50) << QPointF(150, 150) << QPointF(50, 150);

    // Create a custom polygon item and add it to the scene
    PolygonItem *polygonItem = new PolygonItem(polygon);
    scene->addItem(polygonItem);

    // Set the scene on the QGraphicsView (added via designer)
    ui->graphicsViewMain->setScene(scene);

    // Optionally, set render hints (anti-aliasing or other settings)
    ui->graphicsViewMain->setRenderHint(QPainter::Antialiasing);
}

MainWindow::~MainWindow()
{
    delete ui;
}
