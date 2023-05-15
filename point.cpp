#include "point.h"

Point::Point() {}

Point::Point(double x, double y) :
_x {x},
_y {y} {

}

Point::~Point() {}

std::set<Point> Point::generateRandomDataSetOfSize(unsigned int n) {
    std::set<Point> dataSet;

    for(int i = 0; i < n; i++) {
        Point p = Point::randomlyGenerated();
        dataSet.emplace(p);
    }

    return dataSet;
}

std::ostream &operator<<(std::ostream &out, Point &point) {
    out << "(" << point._x << ", " << point._y << ")";
    return out;
}

bool Point::operator<(const Point &other) const {
    return this->_x < other._x;
}

bool Point::operator==(const Point &other) const {
    return this->_x == other._x && this->_y == other._y;
}

bool Point::operator!=(const Point &other) const {
    return this->_x != other._x || this->_y != other._y;
}


double Point::x() const {
    return _x;
}

double Point::y() const {
    return _y;
}

bool Point::isInlier() const {
    return _isInlier;
}

Point Point::randomlyGenerated() {
    double x = randomCoordinate();
    double y = randomCoordinate();
    return Point(x, y);
}

Point Point::randomlyGeneratedOnXvalue(double xValue) {
    assert(xValue >= 0 && xValue <= 1);

    double y = randomCoordinate();
    return Point(xValue, y);
}

Point Point::randomlyGeneratedOnYvalue(double yValue) {
    assert(yValue >= 0 && yValue <= 1);

    double x = randomCoordinate();
    return Point(x, yValue);
}

void Point::display() {
    Point tmp = scale(WINDOW_WIDTH, WINDOW_HEIGHT);
    Imagine::drawCircle(tmp.x(), tmp.y(), POINT_RADIUS, POINT_COLOR);
}

void Point::displayPoints(const std::set<Point> &points) {
    for(auto point : points) {
        point.display();
    }
}

void Point::display(Imagine::Color color) {
    Point tmp = scale(WINDOW_WIDTH, WINDOW_HEIGHT);
    Imagine::drawCircle(tmp.x(), tmp.y(), POINT_RADIUS, color);
}

double Point::randomCoordinate() {
    return static_cast<double>(rand()) / RAND_MAX;
}

void Point::accept() {
    _isInlier = true;
}

void Point::reject() {
    _isInlier = false;
}

Point Point::scale(double xScale, double yScale) {
    return Point(xScale * this->_x, yScale * this->_y);
}

void Point::addNoise() {
    // clamp to make sure to fit in the [0, 1] space
    _x = (std::min(std::max(_x + generateNoiseValue(), static_cast<double>(0)), static_cast<double>(1)));
    _y = (std::min(std::max(_y + generateNoiseValue(), static_cast<double>(0)), static_cast<double>(1)));

}


std::discrete_distribution<> Point::computeProbabilitiesFor(const std::set<Point> &points) {
    std::set<double> tmp;

    for(auto point : points) {
        if(*this == point) {
            tmp.emplace(0.);
        }
        else {
            double p = (std::exp(-squaredDistance(*this, point)/SQUARED_SIGMA))/Z;
            tmp.emplace(p);
        }
    }

    std::discrete_distribution<> probabilities(tmp.begin(), tmp.end());
    return probabilities;
}

std::vector<bool> Point::computeBooleanConsensusSet(const std::set<Point> &dateSet) {
    std::vector<bool> cs;
    for(auto point : dateSet) {

        cs.emplace_back(*this == point ? true : false);

    }
    return cs;
}

/////////////////////////////////////////////////////////////////////////////


double Point::generateNoiseValue() {
    // for sign
    static auto gen = std::bind(std::uniform_int_distribution<>(0,1),std::default_random_engine());
    // random value in [0, MAX_NOISE]
    double noise = (static_cast<double>(rand()) * MAX_NOISE) / RAND_MAX;
    // final value
    return gen() ? noise : -noise;
}

/////////////////////////////////////////////////////////////////////////////

double squaredDistance(Point p1, Point p2) {
    return std::pow(p1.x() - p2.x(), 2) + std::pow(p1.y() - p2.y(), 2);
}
