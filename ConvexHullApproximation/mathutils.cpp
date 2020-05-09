#include "mathutils.h"

#include "QuickHull/QuickHull.cpp"


std::vector<QPoint> convexHull(const std::vector<QPoint> &i_points)
{
    std::vector<Point> points;
    for (int i = 0; i < i_points.size(); i++)
        points.push_back(Point(i_points[i].x(), i_points[i].y()));

    auto sections = quickHull(points);

    std::vector<QPoint> result;
    for (int i = 0; i < sections.size(); i++){
        result.push_back(QPoint(sections[i].m_point1.m_x, sections[i].m_point1.m_y));
    }

    return result;
}
