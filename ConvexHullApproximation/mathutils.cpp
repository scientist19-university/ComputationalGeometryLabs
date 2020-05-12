#include "mathutils.h"

#include "QuickHull/QuickHull.cpp"
#include "Gaussian/Gaussian.cpp"

#include <cmath>

std::vector<QPointF> convexHull(const std::vector<QPointF> &i_points)
{
    std::vector<Point> points;
    for (int i = 0; i < i_points.size(); i++)
        points.push_back(Point(i_points[i].x(), i_points[i].y()));

    auto sections = quickHull(points);

    std::vector<QPointF> result;
    for (int i = 0; i < sections.size(); i++){
        result.push_back(QPoint(sections[i].m_point1.m_x, sections[i].m_point1.m_y));
    }

    return result;
}

std::vector<double> solveSystem(const Matrix &i_matrix)
{
    auto sln = solve(i_matrix);
    return sln.first;
}

QPointF buildBezierCurve(QPointF i_p0, QPointF i_p1, QPointF i_p2, QPointF i_p3, double i_t)
{
    QPointF res = pow((1 - i_t), 3)*i_p0 + 3*pow((1-i_t), 2)*i_t*i_p1 + 3*(1-i_t)*pow(i_t,2)*i_p2 + pow(i_t, 3)*i_p3;
    return res;
}
