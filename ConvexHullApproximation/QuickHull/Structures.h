#pragma once

#include <vector>

struct Point{
  double m_x;
  double m_y;

  Point(double i_x, double i_y);

  void correct(double i_epsilon);
  bool operator ==(const Point& i_point);

//  Point operator +(const Point& i_point);
//  Point operator -(const Point& i_other);
};

const Point NaNPoint = Point(1e9, 1e9);

enum EIntersectionType {
  NoIntersection,
  OnePoint,
  InfinityPoints
};

struct Section {
  Point m_point1;
  Point m_point2;

  Section(const Point& i_p1, const Point& i_p2);

  EIntersectionType getIntersectionType(const Section& i_section) const;
  Point getIntersectionPoint(const Section& i_section) const;
  bool containsPoint(const Point& i_point) const;
  double distance() const;
};

struct Line {
  double m_a;
  double m_b;
  double m_c;

  Line(double i_a, double i_b, double i_c);
  Line(const Point& i_p1, const Point& i_p2);
  Line(const Section& i_section);

  static EIntersectionType getIntersectionType(const Line& i_line1, const Line& i_line2);
  static Point getIntersectionPoint(const Line& i_line1, const Line& i_line2);
};


using PolygonVertexes = std::vector<Point>;


