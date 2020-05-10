#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

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
    Ui::MainWindow *ui;
    QGraphicsScene* mp_scene;

    std::vector<QPoint> m_points;
    std::vector<QPoint> m_ch_points; // points in convex hull

    //control points for Bezier spline
    std::vector<QPoint> m_A;
    std::vector<QPoint> m_B;
};

#endif // MAINWINDOW_H
