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

namespace HermitSpline{

    double h00(double t){
        return 2*pow(t, 3) - 3*pow(t, 2) + 1;
    }

    double h10(double t){
        return pow(t, 3) - 2*pow(t, 2) + t;
    }

    double h01(double t){
        return -2*pow(t, 3) + 3*pow(t, 2);
    }

    double h11(double t){
        return pow(t, 3) - pow(t, 2);
    }

    double t(QPointF i_p0, QPointF i_p1, double i_x){
        return (i_x - i_p0.x())/(i_p1.x() - i_p0.x());
    }
}

double buildHermitSpline(QPointF i_p0, QPointF i_p1, double i_m0, double i_m1, double i_x)
{
    double t = HermitSpline::t(i_p0, i_p1, i_x);
    double diff = i_p1.x() - i_p0.x();

    return HermitSpline::h00(t)*i_p0.y() + HermitSpline::h10(t)*diff*i_m0 +
           HermitSpline::h01(t)*i_p1.y() + HermitSpline::h11(t)*diff*i_m1;
}


namespace Derivaties{

    double twoPointsDiff(QPointF a, QPointF b){
        return (b.y() - a.y())/(b.x() - a.x())/2;
    }
}

std::vector<double> findDerivatives(std::vector<QPointF> i_points)
{
    size_t n = i_points.size();
    std::vector<double> res(n);

    res[0] = 0;
    res[n-1] = 0;

    for (size_t i = 1; i <= n-2; i++)
        res[i] = Derivaties::twoPointsDiff(i_points[i], i_points[i+1]) + Derivaties::twoPointsDiff(i_points[i-1], i_points[i]);

    return res;
}
