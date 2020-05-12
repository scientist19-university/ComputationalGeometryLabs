#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <QPoint>
#include <vector>

#include "Gaussian/Matrix.h"

std::vector<QPointF> convexHull(const std::vector<QPointF>& i_points);
std::vector<double> solveSystem(const Matrix& i_matrix);
QPointF buildBezierCurve(QPointF i_p0, QPointF i_p1, QPointF i_p2, QPointF i_p3, double i_t);

#endif // MATHUTILS_H
