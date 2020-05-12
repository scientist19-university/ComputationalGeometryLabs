#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <memory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void placeRandomPoints();
    void findConvexHull();
    void buildSpline();

private:
    void removeEdges();
    void removeSpline();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* mp_scene;

    std::vector<QPointF> m_points;
    std::vector<QPointF> m_ch_points; // points in convex hull

    //control points for Bezier spline
    std::vector<QPointF> m_A;
    std::vector<QPointF> m_B;

    // scene items
    std::vector<std::unique_ptr<QGraphicsItem>> m_edges;
    std::vector<std::unique_ptr<QGraphicsItem>> m_bezier_spline;
};

#endif // MAINWINDOW_H
