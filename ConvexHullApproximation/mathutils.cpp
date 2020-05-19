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

double BernsteinPolynomial(size_t i, size_t n, double x){
    size_t i_fact = 1,
           div_fact = 1;

    for (size_t j = 1; j <= i; j++){
        i_fact *= j;
        div_fact *= (n - j + 1);
    }

    double res = div_fact/i_fact;
    res *= pow(x, i) * pow(1 - x, n - i);

    return res;
}

QVector3D buildBezierSurface(const std::vector<std::vector<QVector3D>> &i_points, double u, double v)
{
    size_t n = i_points.size() - 1,
           m = i_points[0].size() - 1;

    QVector3D res(0, 0, 0);
    for (size_t i = 0; i <= n; i++)
        for (size_t j = 0; j <= m; j++){
            double b1 = BernsteinPolynomial(i, n, u);
            double b2 = BernsteinPolynomial(j, m, v);

            res += b1 * b2 * i_points[i][j];
        }

    return res;
}
