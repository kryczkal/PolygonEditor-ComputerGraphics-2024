#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsScene>
#include <QMainWindow>

#include "PolygonItem.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    public slots:
    void DeletePolygon();
    void AddVertex();

    private:
    QGraphicsScene *scene;
    PolygonItem *polygonItem;
};
#endif // MAINWINDOW_H
