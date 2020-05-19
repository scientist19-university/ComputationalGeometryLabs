#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QGraphicsScene>
#include <QVector3D>
#include <Qt3DWindow>

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

    void placePointsForBezierSurface();
    void drawBezierSurface();

    void removeAll();

private:
    void removeConvexHullEdges();
    void removeBezierSpline();
    void removeHermitSpline();
    void redraw();
    void placePoints(const std::vector<QPointF>& i_points);

    void setup3DScene();

    void addPointTo3DScene(QVector3D i_point, double i_radius, QColor i_color, bool i_low_quality);

private:
    Ui::MainWindow *ui;
    QGraphicsScene* mp_scene;

    Qt3DExtras::Qt3DWindow *mp_3dview;
    Qt3DCore::QEntity *mp_root_entity;

    std::vector<QPointF> m_points;
    std::vector<QPointF> m_ch_points; // points in convex hull

    //control points for Bezier spline
    std::vector<QPointF> m_A;
    std::vector<QPointF> m_B;

    // scene items
    std::vector<std::unique_ptr<QGraphicsItem>> m_edges;
    std::vector<std::unique_ptr<QGraphicsItem>> m_bezier_spline;
    std::vector<std::unique_ptr<QGraphicsItem>> m_hermit_spline;


    std::vector<QPointF> m_points_for_Hermit_spline;
    std::vector<std::vector<QVector3D>> m_points_for_Bezier_surface;
};

#endif // MAINWINDOW_H
