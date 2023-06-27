/**
 * Author        : Lysandre M. (lysandre.macke@enpc.fr)
 * Created       : 04-27-2023
 * Last modified : 05-31-2023 */

#ifndef LINE_H
#define LINE_H

#include "point.h"
#include "settings.h"
#include <float.h>
#include <Imagine/Graphics.h>
#include <omp.h>


#define INFTY DBL_MAX // approximation of infinity
                      // (isn't really taken into account in calculations)
#define LINE_COLOR Imagine::BLACK


/** Represents a line on the 2D space.
 *  A line is constructed by 2 points but has infinite length.
 *  We work on the finite space [0, 1]x[0, 1].
 *
 *  If displayed on screen, consider that the y axis is going down. */
class Line {
public:
    /** Constructors */
    Line(Point p1, Point p2);

    Line();

    Line(const Line& other);

    Line(double a, double b);

    /**
     * Factory method that creates and returns a randomy generated
     * Line object.
     *
     * @return a randomly generated Line object.
     */
    static Line randomlyGenerated();

    /** Destructor */
    ~Line();

    /** Stream operator << redefinition. */
    friend std::ostream &operator<<(std::ostream &out, Line &line);

    /**
     * < operator redefinition for Line objects.
     *
     * @param other the comparing Line.
     * @return true if the current line is < than the other.
     */
    bool operator<(const Line &other) const;

    /**
     * == operator redefinition for Line objects.
     *
     * @param other
     * @return
     */
    bool operator==(const Line &other) const;


    /** Accessor for private field _a. */
    double a() const;

    /** Accessor for private field _b. */
    double b() const;

    /** Accessor for private field _p1. */
    Point p1() const;

    /** Accessor for private field _p2. */
    Point p2() const;

    /** Returns the squared distanc seperating the Line's 2 points. */
    double squaredLength();



    /**
     * Generates n random inlier points that matches with the line model.
     * Points are generated with noise.
     *
     * @return set of points that matches the line model.
     */
    std::set<Point> generateRandomInliers(unsigned int n);

    /** Screen display of a line. */
    void display(int windowWidth, int windowHeight);

    /** Screen display of a line with a certain color. */
    void display(Imagine::Color color);

    /** Returns the Minimal Sample Set Size of the model.
     *  For a Line object, this constant is set to 2. */
    int mmss();

    /** Value of the Preferencefunction according to the given point. */
    double PFValue(const Point &p);

    /**
     * Returns a set of points representing a star model.
     */
    static std::set<Point> generateStarModel();

    /**
     * Find line with least square method.
     *
     * @param points
     * @return
     */
    static Line leastSquares(const std::vector<std::shared_ptr<Point>> &points);


private:
    // private methods

    /** Returns a randomly selected point from the current line.
     *  Guaranties that the returned point is in [0, 1]x[0, 1]. */
    Point randomPoint();

    // attributes

    Point _p1;      // first point
    Point _p2;      // second point

    // ax + b
    double _a;
    double _b;
};

/** Returns the (shortest) distance from a given point to a given line. */
double distance(Line line, Point point);


/**
 * Compute the preference set of the point, which stores either the point matches with each model
 * or not.
 *
 * @param modelSet a set of models
 * @return a vector of boolean. The i-th element of the vector corresponds to the i-th element
 * of the entry set.
 */
std::vector<bool> computePreferenceSetFor(const Point &point, const std::set<Line> &modelSet);

/**
 * Compute the preference function of the point, which stores the matching degree for each model.
 *
 * @param pointt the point
 * @param models the models
 * @return a vector of double (between 0 and 1). The i-th element of the vector corresponds to
 * the i-th element of the entry set.
 */
std::vector<double> computePreferenceFunctionFor(
        const Point &point,
        const std::vector<Line> &models
        );
#endif // LINE_H
