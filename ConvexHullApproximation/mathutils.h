#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <vector>
#include <QPoint>
#include <QVector3D>

#include "Gaussian/Matrix.h"

std::vector<QPointF> convexHull(const std::vector<QPointF>& i_points);
std::vector<double> solveSystem(const Matrix& i_matrix);
std::vector<double> findControlPoints(const std::vector<double>& i_q);
QPointF buildBezierCurve(QPointF i_p0, QPointF i_p1, QPointF i_p2, QPointF i_p3, double i_t);
double buildHermitSpline(QPointF i_p0, QPointF i_p1, double i_m0, double i_m1, double i_x);
std::vector<double> findDerivatives(std::vector<QPointF> i_points);
QVector3D buildBezierSurface(const std::vector<std::vector<QVector3D>> &i_points, double u, double v);

#endif // MATHUTILS_H
