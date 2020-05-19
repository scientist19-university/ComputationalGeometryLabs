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
    void drawConvexHull();
    void drawBezierSpline();

    void placePointsForHermitSpline();
    void drawHermitSpline();
    //
    //void placePointsForBezierSurface();
    //void buildBezierSurface();

private:
    void removeConvexHullEdges();
    void removeBezierSpline();
    void removeHermitSpline();
    void removeAll();
    void redraw();
    void placePoints(const std::vector<QPointF>& i_points);

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
    std::vector<std::unique_ptr<QGraphicsItem>> m_hermit_spline;

    std::vector<QPointF> m_points_for_Hermit_spline = {{100, 100}, {200, 200}, {400, 200}, {500, 300}, {600, 100}, {700, 200}, {800, 400}, {900, 400}, {1000, 300}};
};

#endif // MAINWINDOW_H
