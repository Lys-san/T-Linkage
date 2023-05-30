/**
 * Author        : Lysandre M. (lysandre.macke@enpc.fr)
 * Created       : 04-27-2023
 * Last modified : 05-11-2023 */

#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <cassert>
#include <random>
#include <Imagine/Graphics.h>
#include <map>
#include <set>

#include "window.h"
#include "settings.h"

#define POINT_RADIUS         1
#define OUTLIER_COLOR        Imagine::BLACK
#define INLIER_COLOR         Imagine::RED
#define POINT_COLOR          isInlier() ? INLIER_COLOR : OUTLIER_COLOR
#define MAX_POINTS_ON_SCREEN 2048

/** Represents a point in the 2D space.
 *  We work on the finite space [0, 1]x[0, 1].
 *
 * If displayed on screen, consider that the y axis is going down. */
class Point
{
public:
    /** Default constructor */
    Point();

    /** Constructor */
    Point(double x, double y);

    /** Destructor */
    ~Point();

    /**
     * Factory method to generate n random points, returned as an immutable set.
     *
     * @param n the size of the set to be generated
     * @return a set of n random points
     */
    static std::set<Point> generateRandomDataSetOfSize(unsigned int n);

    /** Stream operator << redefinition. */
    friend std::ostream &operator<<(std::ostream &out, Point &point);

    /** < operator redefinition. */
    bool operator<(const Point &other) const;

    /**
     * == operator redefinition for Point objects.
     *
     * @param other the comparing point
     * @return true if the points have the same coordinate and false otherwise.
     */
    bool operator==(const Point &other) const;

    /**
     * != operator redefinition for Point objects.
     *
     * @param other the comparing point
     * @return true if the points have different coordinate and false otherwise.
     */
    bool operator!=(const Point &other) const;

    /** Accessor for private _x field. */
    double x() const;

    /** Accessor for private _y field. */
    double y() const;

    bool isInlier() const;


    /** Factory method for random point generation. */
    static Point randomlyGenerated();

    /** Factory method for random point generation on a
     * given X coordinate.
     * Basically, returns a point (xValue, random)*/
    static Point randomlyGeneratedOnXvalue(double xValue);

    /** Factory method for random point generation on a
     * given Y coordinate.
     * Basically, returns a point (xValue, random)
     *
     * @param yValue the y value taken by the generated point
     * @return random point of given Y coordinate
     */
    static Point randomlyGeneratedOnYvalue(double yValue);

    /** Returns a double value between 0 and 1.
     *
     * @return a double value between 9 and 1.
     */
    static double randomCoordinate();

    /** Screen display. */
    void display();

    /**
     * Displays given set of points on the current window.
     *
     * @param points the set of points to display
     */
    static void displayPoints(const std::set<Point> &points);

    /** Screen display with a given color */
    void display(Imagine::Color color);

    /** Changes value of boolean _isInlier to true. */
    void accept();

    /** Changes value of boolean _isInlier to false. */
    void reject();

    /** Returns a point created by scaling the current point according
     *  to the given xScale and yScale parameters.
     *  This function is to be used only for screen display. */
    Point scale(double xScale, double yScale);

    /** Disturb the coordinate of the point. */
    void addNoise();

    /** Computes and return a map associating each point from the given set to its
     *  probability of being drawn after the current point.
     *
     * @param points the set of points to compute probability for
     * @return a map associating each point from the given set to its probability of
     * being draw after th current point.
     */
    std::discrete_distribution<> computeProbabilitiesFor(const std::set<Point> &points);

    std::vector<bool> computeBooleanConsensusSet(const std::set<Point> &dateSet);


private:
    // private methods

    /** Returns a double value in [-MAX_NOISE, MAX_NOISE]. */
    double generateNoiseValue();


    // private attributes

    double _x;
    double _y;

    // does the point matched with a searched model ? (default : false)
    bool _isInlier = false;
};

////////////////////////////////////////////////////////////////////////////////////////

/** Squared euclidian distance between 2 points. */
double squaredDistance(Point p1, Point p2);

#endif // POINT_H
