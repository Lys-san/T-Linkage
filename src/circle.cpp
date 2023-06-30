#include "circle.h"

Circle::Circle() {}

Circle::Circle(const Point &p, double radius):
    _p {p},
    _r {radius} {}

Circle::Circle(const Point &a, const Point &b, const Point &c) {
    auto attributes = circleAttributesFromPoints(a, b, c);
    _p = attributes.first;
    _r = attributes.second;
}

Circle::~Circle() {};

std::pair<Point, double> Circle::circleAttributesFromPoints(const Point &a, const Point &b, const Point &c) {
    assert(!areAligned({a, b, c}));

    auto a_squaredNorm = a.x()*a.x() + a.y()*a.y();
    auto b_squaredNorm = b.x()*b.x() + b.y()*b.y();
    auto c_squaredNorm = c.x()*c.x() + c.y()*c.y();

    auto py = 0.5*(b_squaredNorm - c_squaredNorm) - ((b.x() - c.x())*(0.5*(a_squaredNorm - b_squaredNorm)))/(a.x() - b.x());
    py /= b.y() - c.y() - ((b.x() - c.x())*(a.y() - b.y()))/(a.x() - b.x());

    auto px = 0.5*(a_squaredNorm - b_squaredNorm) - py*(a.y() - b.y());
    px /= a.x() - b.x();

    auto p = Point(px, py);

    auto r = std::sqrt(squaredDistance(p, a));

    return std::make_pair(p, r);
}

Point Circle::p() const {
    return _p;
}

double Circle::r() const {
    return _r;
}

void Circle::display(int windowWidth, int windowHeight) {
    auto tmp = _p.scale(windowWidth, windowHeight);
    Imagine::drawCircle(tmp.x(), tmp.y(), _r * (windowWidth + windowHeight)/2, Imagine::BLACK);
}

double Circle::PFValue(const Point &p) {
    return distance(*this, p) < 5*TAU ? exp(-distance(*this, p)/TAU) : 0;
}

//////////////////////////////////////////////////////////////////

double distance(Circle circle, Point point) {
    return std::abs(sqrt(squaredDistance(circle.p(), point)) - circle.r());
}
