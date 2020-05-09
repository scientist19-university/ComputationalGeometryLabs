#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mathutils.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mp_scene = new QGraphicsScene();
    ui->graphicsView->setScene(mp_scene);

    connect(ui->randomButton, &QPushButton::clicked, this, &MainWindow::placeRandomPoints);
    connect(ui->ConvexHullButton, &QPushButton::clicked, this, &MainWindow::findConvexHull);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::placeRandomPoints()
{
    int number = 10;

    m_points.clear();
    m_ch_points.clear();
    mp_scene->clear();

    auto view_rect = ui->graphicsView->rect();
    int view_width = view_rect.width() - 10,
        view_height = view_rect.height() - 10;

    for (int i = 0; i < number; i++){
        QPoint point = QPoint(rand() % view_width - view_width/2,
                              rand() % view_height - view_height/2);
        m_points.push_back(point);

        mp_scene->addEllipse(point.x()-2, point.y()-2, 5, 5, QPen(), QBrush(Qt::SolidPattern));
    }
}

void MainWindow::findConvexHull()
{
    m_ch_points = convexHull(m_points);

    QPen pen = QPen(Qt::red);
    pen.setWidth(2);
    for (int i = 1; i < m_ch_points.size(); i++){
        mp_scene->addLine(QLineF(m_ch_points[i-1], m_ch_points[i]), pen);
    }

    mp_scene->addLine(QLineF(m_ch_points[m_ch_points.size() - 1], m_ch_points[0]), pen);
}
