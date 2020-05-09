#include <math.h>
#include "Structures.h"

#define sqr(x) (x)*(x)

namespace {
  const double eps = 1e-8;
  bool isEqual(double i_a, double i_b) {
    return (fabs(i_a - i_b) < eps);
  }
}

Point::Point(double i_x, double i_y)
: m_x(i_x),
  m_y(i_y)
{
}

void Point::correct(double i_epsilon)
{
  m_y += i_epsilon;
}

bool Point::operator==(const Point & i_point)
{
  return isEqual(m_x, i_point.m_x) && isEqual(m_y, i_point.m_y);
}

Section::Section(const Point & i_p1, const Point & i_p2)
  : m_point1(i_p1),
    m_point2(i_p2)
{
}

EIntersectionType Section::getIntersectionType(const Section & i_section) const
{
  auto result_for_line = Line::getIntersectionType(*this, i_section);
  if (result_for_line != EIntersectionType::OnePoint)
    return result_for_line;

  auto intersection_point = Line::getIntersectionPoint(*this, i_section);
  if (this->containsPoint(intersection_point) && i_section.containsPoint(intersection_point))
    return EIntersectionType::OnePoint;

  return EIntersectionType::NoIntersection;
}

Point Section::getIntersectionPoint(const Section & i_section) const
{
  return Line::getIntersectionPoint(*this, i_section);
}

bool Section::containsPoint(const Point & i_point) const
{
  Section s1 = Section(i_point, m_point1);
  Section s2 = Section(i_point, m_point2);

  return isEqual(distance(), s1.distance() + s2.distance());
}

double Section::distance() const
{
  return sqrt(sqr(m_point2.m_x - m_point1.m_x) + sqr(m_point2.m_y - m_point1.m_y));
}

Line::Line(double i_a, double i_b, double i_c)
  : m_a(i_a),
    m_b(i_b),
    m_c(i_c)
{
}

Line::Line(const Point & i_p1, const Point & i_p2)
  : m_a(i_p1.m_y - i_p2.m_y),
    m_b(i_p2.m_x - i_p1.m_x),
    m_c(i_p1.m_x*i_p2.m_y - i_p2.m_x*i_p1.m_y)
{
}

Line::Line(const Section & i_section)
  :Line(i_section.m_point1, i_section.m_point2)
{
}

EIntersectionType Line::getIntersectionType(const Line & i_line1, const Line & i_line2)
{
  double a1 = i_line1.m_a,
         b1 = i_line1.m_b,
         c1 = i_line1.m_c,
         a2 = i_line2.m_a,
         b2 = i_line2.m_b,
         c2 = i_line2.m_c;

  double denominator = a1 * b2 - a2 * b1;
  
  if (isEqual(denominator, 0)) {
    auto d1 = a1 * c2 - a2 * c1;
    auto d2 = b1 * c2 - b2 * c1;
    
    if (isEqual(d1, 0) && isEqual(d2, 0))
      return EIntersectionType::InfinityPoints;

    return EIntersectionType::NoIntersection;
  }

  return EIntersectionType::OnePoint;
}

Point Line::getIntersectionPoint(const Line & i_line1, const Line & i_line2)
{
  double a1 = i_line1.m_a,
         b1 = i_line1.m_b,
         c1 = i_line1.m_c,
         a2 = i_line2.m_a,
         b2 = i_line2.m_b,
         c2 = i_line2.m_c;

  double denominator = a1 * b2 - a2 * b1;

  auto dx = c1 * b2 - c2 * b1;
  auto dy = a1 * c2 - a2 * c1;

  return Point(-dx / denominator, -dy / denominator);
}
