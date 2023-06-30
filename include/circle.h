/**
 * Author        : Lysandre M. (lysandre.macke@enpc.fr)
 * Created       : 06-29-2023
 * Last modified : 06-30-2023 */


#ifndef CIRCLE_H
#define CIRCLE_H

#include "line.h"

/**
 * Represents a circle in the 2D space.
 * We work on th finite space [0, 1]x[0, 1].
 *
 * If displayed on screen, consider that the  axis is going down.
 */
class Circle {
public:
    /** Default constructor */
    Circle();

    Circle(const Point &p, double radius);

    Circle(const Point &a, const Point &b, const Point &c);

    /** Destructor*/
    ~Circle();

    /** Screen display of a circle. */
    void display(int windowWidth, int windowHeight);





private:
    // private methods
    /**
     * Returns the center and radius of the circumscribed circle of the triangle formed by the
     * 3 given points a, b and c.
     * Asserts that  the 3 points are NOT aligned.
     *
     * @param a first point
     * @param b second point
     * @param c third point
     * @return
     */
    std::pair<Point, double> circleAttributesFromPoints(const Point &a, const Point &b, const Point &c);

    // private attributes
    Point _p; // center
    double _r; // radius

};

#endif // CIRCLE_H
