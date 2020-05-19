#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mathutils.h"
#include "uipoint.h"

#include <QGraphicsEllipseItem>

namespace{

    std::vector<double> findControlPoints(const std::vector<double>& i_q){
        size_t n = i_q.size() - 1;
        std::vector<std::vector<double>> init;


        for (size_t i = 1; i <= n-1; i++){
            std::vector<double> row(2*n + 1, 0);

            row[2*i] = 1;
            row[2*(i-1) + 1] = 1;
            row[2*n] = 2*i_q[i];
            init.push_back(row);

            row.clear();
            row.resize(2*n + 1, 0);

            row[2*(i-1)] = 1;
            row[2*(i-1) + 1] = -2;
            row[2*i] = 2;
            row[2*i + 1] = -1;
            init.push_back(row);
        }

       std::vector<double> row(2*n + 1, 0);
       row[0] = 2;
       row[1] = -1;
       row[2*n] = i_q[0];
       init.push_back(row);

       row.clear();
       row.resize(2*n + 1, 0);
       row[2*(n-1) + 1] = 2;
       row[2*(n-1)] = -1;
       row[2*n] = i_q[n];
       init.push_back(row);

       auto sln = solveSystem(Matrix(init));
       return sln;
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mp_scene = new QGraphicsScene();
    ui->graphicsView->setScene(mp_scene);

    connect(ui->randomButton, &QPushButton::clicked, this, &MainWindow::placeRandomPoints);
    connect(ui->ConvexHullButton, &QPushButton::clicked, this, &MainWindow::drawConvexHull);
    connect(ui->approxButton, &QPushButton::clicked, this, &MainWindow::drawBezierSpline);

    connect(ui->placePointsButton, &QPushButton::clicked, this, &MainWindow::placePointsForHermitSpline);
    connect(ui->buildHermiteSplineButton, &QPushButton::clicked, this, &MainWindow::drawHermitSpline);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::placeRandomPoints()
{
    int number = 10;

    auto view_rect = ui->graphicsView->rect();
    int view_width = view_rect.width()*3/4,
        view_height = view_rect.height()*3/4;

    std::vector<QPointF> points;
    for (int i = 0; i < number; i++){
        QPoint point = QPoint(rand() % view_width + view_width*1/8,
                              rand() % view_height + view_height*1/8);
        points.push_back(point);
    }

    placePoints(points);
}

void MainWindow::drawConvexHull()
{
    removeConvexHullEdges();
    m_ch_points = convexHull(m_points);

    QPen pen = QPen(Qt::red);
    pen.setWidth(2);
    for (size_t i = 1; i < m_ch_points.size(); i++){
        auto* line = mp_scene->addLine(QLineF(m_ch_points[i-1], m_ch_points[i]), pen);
        m_edges.push_back(std::unique_ptr<QGraphicsItem>(line));
    }

    auto* line = mp_scene->addLine(QLineF(m_ch_points[m_ch_points.size() - 1], m_ch_points[0]), pen);
    m_edges.push_back(std::unique_ptr<QGraphicsItem>(line));
}

void MainWindow::drawBezierSpline()
{
    removeBezierSpline();
    std::vector<double> q_x, q_y;

    if (m_ch_points.empty())
        return;

    m_ch_points.push_back(m_ch_points[0]);
    m_ch_points.push_back(m_ch_points[1]);

    for (size_t i = 0; i < m_ch_points.size(); i++){
        q_x.push_back(m_ch_points[i].x());
        q_y.push_back(m_ch_points[i].y());
    }

    auto x = findControlPoints(q_x);
    auto y = findControlPoints(q_y);

    m_A.clear();
    m_B.clear();
    for (size_t i = 0; i < m_ch_points.size() - 1; i++){
        m_A.push_back(QPointF(x[2*i], y[2*i]));
        m_B.push_back(QPointF(x[2*i+1], y[2*i+1]));
    }


    for (size_t i =1; i < m_A.size(); i++){

        for(double t = 0; t <= 1; t += 0.005){
            QPointF p = buildBezierCurve(m_ch_points[i], m_A[i], m_B[i], m_ch_points[i+1], t);
            auto* point = mp_scene->addEllipse(p.x()-1, p.y()-1, 2, 2, QPen(Qt::blue), QBrush(Qt::SolidPattern));
            m_bezier_spline.push_back(std::unique_ptr<QGraphicsItem>(point));
        }
    }

    m_ch_points.erase(m_ch_points.end() - 1);
    m_ch_points.erase(m_ch_points.end() - 1);
}

void MainWindow::placePointsForHermitSpline()
{
    placePoints(m_points_for_Hermit_spline);
}

void MainWindow::drawHermitSpline()
{
    removeHermitSpline();

    std::vector<double> derivatives = findDerivatives(m_points);
    for (size_t i = 0; i < m_points.size() - 1; i++){
        QPointF a = m_points[i],
                b = m_points[i+1];

        double length = b.x() - a.x();
        for (double x = a.x(); x <= b.x(); x += 0.001*(length)){
            double y = buildHermitSpline(a, b, derivatives[i], derivatives[i+1], x);

            QPointF p(x, y);
            auto* point = mp_scene->addEllipse(p.x()-1, p.y()-1, 2, 2, QPen(QColor("#00b300")), QBrush(Qt::SolidPattern));
            m_hermit_spline.push_back(std::unique_ptr<QGraphicsItem>(point));
        }
    }
}


void MainWindow::removeConvexHullEdges()
{
    for (auto& p_item : m_edges)
        mp_scene->removeItem(p_item.get());
    m_edges.clear();
}

void MainWindow::removeBezierSpline()
{
    for (auto& p_item : m_bezier_spline)
        mp_scene->removeItem(p_item.get());
    m_bezier_spline.clear();
}

void MainWindow::removeHermitSpline()
{
    for (auto& p_item : m_hermit_spline)
        mp_scene->removeItem(p_item.get());
    m_hermit_spline.clear();
}

void MainWindow::removeAll()
{
    removeConvexHullEdges();
    removeBezierSpline();
    removeHermitSpline();
    mp_scene->clear();
    m_points.clear();
    m_ch_points.clear();
}

void MainWindow::redraw()
{
    if (ui->tabWidget->currentWidget() == ui->bezier_approx_tab){
        drawConvexHull();
        drawBezierSpline();
    }
    else if (ui->tabWidget->currentWidget() == ui->hermite_spline_tab){
        drawHermitSpline();
    }
}

void MainWindow::placePoints(const std::vector<QPointF> &i_points)
{
    removeAll();

    for (size_t i = 0; i < i_points.size(); i++){

        QPointF point = i_points[i];
        m_points.push_back(point);

        auto* item = new UIPoint(point.x(), point.y());
        mp_scene->addItem(item);

        connect(item, &UIPoint::positionChanged, [this, i](QPointF i_new_point){
            m_points[i] = i_new_point + QPointF(2, 2);
            redraw();
        });
    }
}
