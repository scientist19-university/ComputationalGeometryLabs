#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mathutils.h"
#include "uipoint.h"

#include <QGraphicsEllipseItem>

#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QForwardRenderer>
#include <QtGui/QScreen>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DCore/QTransform>
#include <Qt3DCore/QEntity>
#include <Qt3DInput/QInputAspect>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DRender/QCamera>
#include <Qt3DExtras/QFirstPersonCameraController>
#include <Qt3DRender/QPointLight>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mp_scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(mp_scene);

    setup3DScene();

    connect(ui->randomButton, &QPushButton::clicked, this, &MainWindow::placeRandomPoints);
    connect(ui->ConvexHullButton, &QPushButton::clicked, this, &MainWindow::drawConvexHull);
    connect(ui->approxButton, &QPushButton::clicked, this, &MainWindow::drawBezierSpline);

    connect(ui->placePointsButton, &QPushButton::clicked, this, &MainWindow::placePointsForHermitSpline);
    connect(ui->buildHermiteSplineButton, &QPushButton::clicked, this, &MainWindow::drawHermitSpline);

    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::removeAll);

    connect(ui->placeSurfacePointsButton, &QPushButton::clicked, this, &MainWindow::placePointsForBezierSurface);
    connect(ui->buildBezierSurfaceButton, &QPushButton::clicked, this, &MainWindow::drawBezierSurface);

    //initializing defined points
    m_points_for_Hermit_spline = {{100, 100}, {200, 200}, {400, 200}, {500, 300}, {600, 100}, {700, 200}, {800, 400}, {900, 400}, {1000, 300}};
    m_points_for_Bezier_surface = { { {1, 1, 2}, {2, 3, 2}, {3, 2, 1}, {4, 4, 1} },
                                    { {5, 1, 1}, {6, 4, 2}, {7, 3, 2}, {9, 1, 0} } };
}

MainWindow::~MainWindow()
{
    removeAll();

    delete mp_3dview;
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
        for (double x = a.x(); x <= b.x(); x += 0.002*(length)){
            double y = buildHermitSpline(a, b, derivatives[i], derivatives[i+1], x);

            QPointF p(x, y);
            auto* point = mp_scene->addEllipse(p.x()-1, p.y()-1, 2, 2, QPen(QColor("#00b300")), QBrush(Qt::SolidPattern));
            m_hermit_spline.push_back(std::unique_ptr<QGraphicsItem>(point));
        }
    }
}

void MainWindow::placePointsForBezierSurface()
{
    for (auto& array: m_points_for_Bezier_surface)
        for (auto& point : array)
            addPointTo3DScene(point, 0.05, Qt::red, false);
}

void MainWindow::drawBezierSurface()
{
    for (double u = 0; u <= 1; u += 0.015)
        for (double v = 0; v <= 1; v += 0.015){
            QVector3D point = buildBezierSurface(m_points_for_Bezier_surface, u, v);
            addPointTo3DScene(point, 0.02, QColor(0, 0, 0, 5), true);
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

void MainWindow::setup3DScene()
{
    mp_3dview = new Qt3DExtras::Qt3DWindow();

    mp_3dview->defaultFrameGraph()->setClearColor(QColor(QRgb(0xffffff)));
    QWidget *container = QWidget::createWindowContainer(mp_3dview);
    QSize screenSize = mp_3dview->screen()->size();
    container->setMinimumSize(QSize(200, 100));
    container->setMaximumSize(screenSize);

    ui->d3_tab->layout()->addWidget(container);

    Qt3DInput::QInputAspect *input = new Qt3DInput::QInputAspect;
    mp_3dview->registerAspect(input);

    mp_root_entity = new Qt3DCore::QEntity();
    mp_3dview->setRootEntity(mp_root_entity);

    Qt3DRender::QCamera *cameraEntity = mp_3dview->camera();
    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(0, 0, 20.0f));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));

    Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(mp_root_entity);
    camController->setCamera(cameraEntity);

    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(mp_root_entity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor(Qt::white);
    light->setIntensity(1);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(cameraEntity->position());
    lightEntity->addComponent(lightTransform);
}

void MainWindow::addPointTo3DScene(QVector3D i_point, double i_radius, QColor i_color, bool i_low_quality)
{
    Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh();
    sphereMesh->setRadius(i_radius);
    if (i_low_quality){
        sphereMesh->setRings(2);
        sphereMesh->setSlices(2);
    }

    Qt3DCore::QTransform *sphereTransform = new Qt3DCore::QTransform();
    sphereTransform->setTranslation(QVector3D(i_point.x(), i_point.y(), i_point.z()));

    Qt3DExtras::QPhongMaterial *sphereMaterial = new Qt3DExtras::QPhongMaterial();
    sphereMaterial->setShininess(0);
    sphereMaterial->setDiffuse(i_color);

    Qt3DCore::QEntity *sphereEntity = new Qt3DCore::QEntity(mp_root_entity);
    sphereEntity->addComponent(sphereMesh);
    sphereEntity->addComponent(sphereTransform);
    sphereEntity->addComponent(sphereMaterial);
}
