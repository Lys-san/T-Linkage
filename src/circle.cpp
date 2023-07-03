#include "circle.h"

Circle::Circle() {}

Circle::Circle(const Point &p, double radius):
    _p {p},
    _r {radius} {}

Circle::Circle(const Point &a, const Point &b, const Point &c, int windowWidth, int windowHeight) {
    auto aa = a.scale(windowWidth, windowHeight);
    auto bb = b.scale(windowWidth, windowHeight);
    auto cc = c.scale(windowWidth, windowHeight);

    std::cout << aa << " " << bb << " "<< cc << std::endl;



    auto attributes = circleAttributesFromPoints(aa, bb, cc);

    _p = attributes.first;
    std::cout << "BEFORE==== p : " << _p << std::endl;
    std::cout << "scaling : " << 1./static_cast<double>(windowWidth) << std::endl;


    _p = _p.scale(1./static_cast<double>(windowWidth), 1./static_cast<double>(windowHeight));
    std::cout << "==== p : " << _p << std::endl;
    _r = attributes.second / std::max(windowWidth, windowHeight);
}

Circle::~Circle() {};

bool Circle::operator==(const Circle &other) const {
    return this->_p == other._p && this->_r == other._r;
}

std::pair<Point, double> Circle::circleAttributesFromPoints(const Point &a, const Point &b, const Point &c) {
//    assert(!areAligned({a, b, c}));

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

std::vector<Circle> Circle::drawModels(unsigned int n, const PointPool &dataSet, int windowWidth, int windowHeight) {
    assert(N_MODELS_TO_DRAW <= dataSet.size()/3);

    std::vector<Circle> models;

    while(models.size() < N_MODELS_TO_DRAW) {
        auto insert = true;
        std::vector<Point> circlePoints;
        circlePoints.emplace_back(*dataSet.at(std::rand() % dataSet.size()));

        auto firstPointindex = std::rand() % dataSet.size();
        auto p1 = dataSet[firstPointindex];
        circlePoints.emplace_back(*p1);

        std::discrete_distribution<> d = p1->computeProbabilitiesFor(dataSet.points());
        std::random_device rd;
        std::mt19937 gen(rd());

        int nextPointIndex = d(gen);
        auto p2 = dataSet[nextPointIndex];
        circlePoints.emplace_back(*p2);


        do {
            nextPointIndex = d(gen);
        }
        while(nextPointIndex == firstPointindex);

        auto p3 = dataSet[nextPointIndex];
        circlePoints.emplace_back(*p3);




        auto model = Circle(circlePoints[0], circlePoints[1], circlePoints[2], windowWidth, windowHeight);
        for(auto circle : models) {
            if(model == circle) {
                insert = false;
            }
        }
        if(insert) {
            models.emplace_back(model);
        }
    }
    std::cout<< "[DEBUG] End of random sampling. Generated "
             << models.size()  << " models for total data of size  : " << dataSet.size() << std::endl;
    return models;
}

//////////////////////////////////////////////////////////////////

double distance(Circle circle, Point point) {
    return std::abs(sqrt(squaredDistance(circle.p(), point)) - circle.r());
}
