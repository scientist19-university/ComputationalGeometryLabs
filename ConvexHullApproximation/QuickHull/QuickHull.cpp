#include "Structures.h"

Point findLeftPoint(const std::vector<Point>& i_points) {
  Point res(2e9, 0);
  for (auto& point : i_points)
    if (point.m_x < res.m_x)
      res = point;

  return res;
}

Point findRightPoint(const std::vector<Point>& i_points) {
  Point res(-2e9, 0);
  for (auto& point : i_points)
    if (point.m_x > res.m_x)
      res = point;

  return res;
}

double cross(const Point& p, const Point&  a, const Point& b) {
  return (a.m_x - p.m_x)*(b.m_y - p.m_y) - (a.m_y - p.m_y)*(b.m_x - p.m_x);
}

std::vector<Point> getLeftSidePoints(const std::vector<Point>& i_points, const Point& p1, const Point& p2) {
  std::vector<Point> res;

  res.push_back(p1);
  res.push_back(p2);

  for (auto& point : i_points)
    if (cross(point, p1, p2) > 0.0)
      res.push_back(point);

  return res;
}

Point findPivot(const std::vector<Point>& i_points) {
  auto p1 = i_points[0],
       p2 = i_points[1];
  Point pivot(0, 0);

  double maxcross = -2e9;
  for (size_t i = 2; i < i_points.size(); i++) {
    auto& point = i_points[i];
    auto cross_val = cross(point, p1, p2);
    if (cross_val > maxcross) {
      maxcross = cross_val;
      pivot = point;
    }
  }

  return pivot;
}

void addSections(std::vector<Section>& i_sections, const std::vector<Section>& i_sections_to_add) {
  for (auto& point : i_sections_to_add)
    i_sections.push_back(point);
}

std::vector<Section> qh(const std::vector<Point>& i_points) {

  auto p1 = i_points[0],
       p2 = i_points[1];

  if (i_points.size() == 2) return { Section(p1, p2) };
  
  auto pivot = findPivot(i_points);

  auto res1 = qh(getLeftSidePoints(i_points, p1, pivot));
  auto res2 = qh(getLeftSidePoints(i_points, pivot, p2));

  std::vector<Section> result;

  addSections(result, res1);
  addSections(result, res2);

  return result;
}

std::vector<Section> quickHull(const std::vector<Point>& i_points) {
  auto left_p = findLeftPoint(i_points);
  auto right_p = findRightPoint(i_points);

  auto res1 = qh(getLeftSidePoints(i_points, left_p, right_p));
  auto res2 = qh(getLeftSidePoints(i_points, right_p, left_p));

  std::vector<Section> result;

  addSections(result, res1);
  addSections(result, res2);

  return result;
}
