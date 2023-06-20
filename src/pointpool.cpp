#include "pointpool.h"

PointPool::PointPool() {}

PointPool::~PointPool() {};

bool PointPool::insert(const Point &p) {
//    if(std::find(_points.begin(), _points.end(), p) != _points.end()) {
//        return false; // element is already present => do not insert
//    }

    for(auto point : _points) {
        if(*point == p) {
            return false;
        }
    }

    _points.emplace_back(std::make_shared<Point>(p));
    return true;
}

bool PointPool::insert(const Point &p, int filterValue) {
    for(auto point : _points) {
        if(*point == p) {
            return false;
        }
    }

    if(rand() % filterValue != 0) {
        return false;
    }

    _points.emplace_back(std::make_shared<Point>(p));
    return true;
}

std::shared_ptr<Point> PointPool::at(unsigned int pos) const {
    return _points.at(pos);
}

std::shared_ptr<Point> PointPool::operator[](unsigned int pos) const {
    return _points[pos];
}


PointPool PointPool::generateRandomDataSetOfSize(unsigned int n) {
    PointPool dataSet;
    for(int i = 0; i < n; i++) {
        auto p = Point::randomlyGenerated();
        dataSet.insert(p);
    }

    return dataSet;
}

unsigned long PointPool::size() const {
    return _points.size();
}

std::shared_ptr<Point> PointPool::retrievePointAt(unsigned int pos) {
    auto it = _points.begin() + pos;
    auto returnValue = *it;
    _points.erase(it);
    return returnValue;
}

std::vector<std::shared_ptr<Point>>::iterator PointPool::begin() {
    return _points.begin();
}

std::vector<std::shared_ptr<Point>>::iterator PointPool::end() {
    return _points.end();
}
