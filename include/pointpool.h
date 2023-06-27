/**
 * Author        : Lysandre M. (lysandre.macke@enpc.fr)
 * Created       : 06-13-2023
 * Last modified : 06-15-2023 */

#ifndef POINTPOOL_H
#define POINTPOOL_H

#include "point.h"

/**
 * Represents a set of unique Point objects. To be used once in the progam.
 */
class PointPool {
public:
    /** Constructor */
    PointPool();

    /**
     * Factory method to generate n random points, returned as an immutable set.
     *
     * @param n the size of the set to be generated
     * @return a set of n random points
     */
    static PointPool generateRandomDataSetOfSize(unsigned int n);

    /** Destructor */
    ~PointPool();

    /** Accessor for private field _points. */
    std::vector<std::shared_ptr<Point>> points() const;

    /** Insert new point in pool if not already exists. */
    bool insert(const Point &p);

    /**
     * Insert with a given probability defined as 1/filterValue.
     * The greater the filterValue gets, the lower the probability of
     * the point being inserted gets.
     *
     * The insertion cannot oper if the point is already present.
     */
    bool insert(const Point &p, int filterValue);

    /**
     * Returns a shared pointer to the point at the given position.
     *
     * @param pos the position of the point
     * @return a shared_ptr to the point
     */
    std::shared_ptr<Point> at(unsigned int pos) const;

    /**
     * Returns a shared pointer to the point at the given position.
     *
     * @param pos the position of the point
     * @return a shared_ptr to the point
     */
    std::shared_ptr<Point> operator[](unsigned int pos) const;

    /** Returns size of the pool point. */
    unsigned long size() const;

    /** Returns and erase the point at a given position. */
    std::shared_ptr<Point> retrievePointAt(unsigned int pos);

    /** Returns an iterator to the first point (shared pointer). */
     std::vector<std::shared_ptr<Point>>::iterator begin();

     /** Returns an iterator to the end (shared pointer). */
     std::vector<std::shared_ptr<Point>>::iterator end();

private:
    // private attributes
   std::vector<std::shared_ptr<Point>> _points;
};

#endif // POINTPOOL_H
