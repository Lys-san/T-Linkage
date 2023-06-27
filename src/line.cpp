#include "line.h"

Line::Line() {}


Line::Line(Point p1, Point p2):
    _p1 {p1},
    _p2 {p2} {
    // calculate _a and _b values at initialization
    if(p1.x() == p2.x()) {
        _a = INFTY;
    }
    else {
        _a = (p2.y() - p1.y())/(p2.x() - p1.x());
    }

    _b = p1.y() - _a*p1.x();
}


Line::Line(const Line& other):
    _p1 {other._p1},
    _p2 {other._p2},
    _a {other._a},
    _b {other._b} {

}

Line::Line(double a, double b):
    _a {a},
    _b {b} {
    double x1 = b;
    double x2 = a + b;
    _p1 = Point(0., x1);
    _p2 = Point(1., x2);

}


Line Line::randomlyGenerated() {
    Point p = rand() % 2 == 0 ? Point::randomlyGeneratedOnYvalue(0.) : Point::randomlyGeneratedOnXvalue(0.);
    Point q = rand() % 2 == 0 ? Point::randomlyGeneratedOnYvalue(1.) : Point::randomlyGeneratedOnXvalue(1.);
    return Line(p, q);
}

Line::~Line() {}

double Line::a() const {
    return _a;
}

double Line::b() const {
    return _b;
}

Point Line::p1() const {
    return _p1;
}

Point Line::p2() const {
    return _p2;
}

double Line::squaredLength() {
    return squaredDistance(_p1, _p2);
}

std::set<Point> Line::generateRandomInliers(unsigned int n) {
    std::set<Point> inliers;
    for(int i = 0; i < n; i++) {
        Point pp = randomPoint();
        pp.addNoise();
        inliers.emplace(pp);
    }
    return inliers;
}


std::ostream &operator<<(std::ostream &out, Line &line) {
    out << line.a() << "x + " << line.b();
    return out;
}

bool Line::operator<(const Line &other) const {
    return _a < other._a && _b != other._b; // actually this is a random definition, just to be able to make sets
}

bool Line::operator==(const Line &other) const {
    return _a == other._a && _b == other._b;
}

void Line::display(int windowWidth, int windowHeight) {
    Point tmp1 = _p1.scale(windowWidth, windowHeight);
    Point tmp2 = _p2.scale(windowWidth, windowHeight);
    Imagine::drawLine(tmp1.x(), tmp1.y(), tmp2.x(), tmp2.y(), LINE_COLOR);
}

void Line::display(Imagine::Color color) {
    Point tmp1 = _p1.scale(WINDOW_WIDTH, WINDOW_HEIGHT);
    Point tmp2 = _p2.scale(WINDOW_WIDTH, WINDOW_HEIGHT);
    Imagine::drawLine(tmp1.x(), tmp1.y(), tmp2.x(), tmp2.y(), color);
}

int Line::mmss() {
    return 2;
}

double Line::PFValue(const Point &p) {
    return distance(*this, p) < 5*TAU ? exp(-distance(*this, p)/TAU) : 0;
}

std::set<Point> Line::generateStarModel() {
    Point p1 = Point(1./2, 0);
    Point p2 = Point(0, 1./4);
    Point p3 = Point(1., 1./4);
    Point p4 = Point(1./5, 1.);
    Point p5 = Point(4./5, 1.);

    std::vector<Line> lines;

    lines.emplace_back(Line(p1, p4));
    lines.emplace_back(Line(p4, p3));
    lines.emplace_back(Line(p3, p2));
    lines.emplace_back(Line(p2, p5));
    lines.emplace_back(Line(p5, p1));

    std::set<Point> inliers;

    for(auto line : lines) {
        std::cout << line << std::endl;
        auto tmp = line.generateRandomInliers(N_INLIERS);
        inliers.insert(tmp.begin(), tmp.end());
    }

    return inliers;
}

Line Line::leastSquares(const std::vector<std::shared_ptr<Point>> &points) {
    double xSum = 0.;
    double ySum = 0.;
    double xySum = 0.;
    double xSquaredSum = 0.;


    for(auto point : points) {
        xSum += point->x();
        ySum += point->y();
        xySum += point->x()*point->y();
        xSquaredSum += pow(point->x(), 2);
    }

    double a = (xSum*ySum - points.size()*xySum)/(pow(xSum, 2) - points.size()*xSquaredSum);
    double b = (xySum*xSum - xSquaredSum*ySum)/(pow(xSum, 2) - points.size()*xSquaredSum);
    return Line(a, b);
}


////////////////////////////////////////////////////////////////////////////////////////

Point Line::randomPoint() {
    if(_a == INFTY) {
        return Point::randomlyGeneratedOnXvalue(_p1.x());
    }

    double x = Point::randomCoordinate();
    double y = _a*x + _b;

    // to guaranty that the selectionned point is in [0, 1]²
    if(y < 0 || y > 1) {
        double min = -_b/_a;
        double max = (1. - _b)/_a;

        x *= max - min;
        x += min;
        y = _a*x + _b;
    }

    return Point(x, y);
}

////////////////////////////////////////////////////////////////////////////////////////


double distance(Line line, Point point) {
    if(line.a() == INFTY) {
        return std::abs(line.b() - point.y());
    }

    auto x = point.x();
    auto y = point.y();

    auto a = line.a();
    auto b = line.b();

    double num = std::abs(a*x -y + b);
    double den = std::sqrt(a*a + 1);
    return num/den;
}

std::vector<double> computePreferenceFunctionFor(const Point &point, const std::vector<Line> &models) {
    std::vector<double> pf;

    for(auto model : models) {
        pf.emplace_back(model.PFValue(point));
    }
    return pf;
}
