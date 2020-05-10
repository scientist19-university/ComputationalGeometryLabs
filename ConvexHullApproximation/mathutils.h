#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <QPoint>
#include <vector>

#include "Gaussian/Matrix.h"

std::vector<QPoint> convexHull(const std::vector<QPoint>& i_points);
std::vector<double> solveSystem(const Matrix& i_matrix);
QPoint buildBezierCurve(QPoint i_p0, QPoint i_p1, QPoint i_p2, QPoint i_p3, double i_t);

#endif // MATHUTILS_H
