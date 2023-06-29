/**
 * Author        : Lysandre M. (lysandre.macke@enpc.fr)
 * Created       : 06-29-2023
 * Last modified : 06-29-2023 */


#ifndef CIRCLE_H
#define CIRCLE_H

#include "point.h"

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


private:
    // private attributes
    Point p; // center
    double r; // radius

};

#endif // CIRCLE_H
