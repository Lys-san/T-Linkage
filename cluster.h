/**
 * Author        : Lysandre M. (lysandre.macke@enpc.fr)
 * Created       : 04-27-2023
 * Last modified : 05-17-2023 */

#ifndef CLUSTER_H
#define CLUSTER_H

#include <iostream>
#include <set>

#include "point.h"
#include "model.h"
#include "line.h"
#include "util.h"


/** Represents a cluster of points, which can eventually be view as a model hypothesis.
 *  A cluster is a vector of points from the 2D space. */
class Cluster {
public:
    /** Constructors */
    Cluster();

    Cluster(const Cluster& other);

    Cluster(Point p);

    Cluster(const std::set<Point> &points);

    /** Destructor */
    ~Cluster();

    /** Factory method that generates N/2 clusters from the given data set. */
    static std::vector<Cluster> clusterizePairs(const std::set<Point> &points);

    /** Factory method that generates N singleton clusters from a data set
     *  of size N */
    static std::vector<Cluster> clusterize(const std::set<Point> &points);


    /** Stream operator << redefinition. */
    friend std::ostream &operator<<(std::ostream &out, Cluster &cluster);

    /**
     * < operator redefinition for Cluster objects.
     * Cluster objects are compared with their size.
     *
     * @param other the comparing cluster.
     * @return true if the current cluster is < than the other.
     */
    bool operator<(const Cluster &other) const;

    /**
     * == operator redefinition for Cluster objects.
     * 2 clusters are equals if their points (vectors) are equals.
     *
     * @param other cluster to compare to
     * @return true if the clusters are ==.
     */
    bool operator==(const Cluster &other) const;

    /** Accessor for private _points field. */
    std::vector<Point> points() const;

    /** Adds point to cluster. */
    void addPoint(Point p);

    /** Accepts all points. */
    void validate();

    /** Rejects all points. */
    void invalidate();

    /** Returns the size of the cluster, i.e. the number of elements
     *  that it contains. */
    int size();

    /**
     * Displays the given vectors, automatically assigning each one a color.
     *
     * @param clusters the clusters to be displayed.
     */
    static void displayClusters(const std::vector<Cluster> &clusters);

    /**
     * Displays the given vectors, automatically assigning each one a color.
     *
     * @param clusters the clusters to be displayed.
     */
    static void displayClustersWithColors(const std::vector<Cluster> &clusters);

    /**
     * Displays only the clusters that are validated.
     *
     * @param clusters the clusters to be displayed.
     */
    static void displayValidated(const std::vector<Cluster> &clusters);

    /**
     * @brief displayModels
     * @param clusters
     */
    static void displayModels(const std::vector<Cluster> &clusters);

    /**
     * Creates a line for clusters of size 2.
     * @return Line object
     */
    Line extractLineModel();

    Point extractPointModel();

    /**
     *  Computes and returns the preference function of the cluster,
     *  defined as the vector of PF minimums for each point contained
     *  in the cluster.
     */
    std::vector<double> computePF(const std::vector<Line> &models);

    /**
     * Returns the intersection of the two given vectors.
     *
     * @param a first vector
     * @param b second vector
     * @return
     */
    static std::set<Line> makeInter(const std::set<Line> &a,
                                       const std::set<Line> &b
                                       );
    /**
     * @return true if all points were validated.
     */
    bool isModel();


private:

    // private methods

    /**
     * Returns the union of the two given vectors.
     *
     * @param a first vector
     * @param b second vector
     * @return
     */
    static std::vector<Line> makeUnion(const std::vector<Line> &a,
                                       const std::vector<Line> &b
                                       );



    // attributes

    std::vector<Point> _points;                // vector of points composing the cluster

};

///////////////////////////////////////////////////////////////////////////////////////////

std::map<Point, int> generatePointIndexes();

/** Generates preference matrix of data set */
std::vector<std::vector<double>> computePM(
        const std::vector<Line> &models,
        const std::set<Point> dataSet
        );

/** Returns the transposate of the given pm. */
std::vector<std::vector<bool>> transposatePM(const std::vector<std::vector<bool>> &pm);

/**
 * Exctracts a map associating a point with a set of models from the given PM.
 * Requires a data set and a set of models.
 *
 * @param models
 * @param pm
 * @return
 */
std::map<Point, std::set<Line>> extractPSfromPM(
        const std::set<Point> &dataSet,
        const std::vector<Line> &models,
        const std::vector<std::vector<bool>> &pm);

/** Returns the Jaccard distance (between 0 and 1) from 2 vectors a and b. */
double jaccard(
        std::set<Line> a,
        std::set<Line> b
        );


/**
 * Returns the tanomoto distance (between 0 and 1) from 2
 * vectors a and b.
 *
 * @param a
 * @param b
 * @return
 */
double tanimoto(
        std::vector<double> a,
        std::vector<double> b
        );

/** Performs linking action and updates given parameters. */
bool link(
        std::vector<Cluster> &clusters,
        std::set<Point> &dataSet,
        const std::vector<std::vector<double>> &pm,
        const std::vector<Line> &models
        );

/** (DEPRECATED) Validates the n biggest clusters contained in the given vector. */
void validateNBiggestClusters(unsigned int n, std::vector<Cluster> &clusters);

/** Validates the biggest clusters contained in the given vector.
  * A cluster is valid if its size is big enough compared to other clusters size. */
void validateBiggestClusters(std::vector<Cluster> &clusters);

/** Validaes the biggest clusters contained in the given vector according to the dataSet size.
 *  A cluster is valid if its size is > minSize. */
void validateBiggestClusters_2(std::vector<Cluster> &clusters, int dataSetSize);


/**
 * Extract models from clusters, asserting that cluster contains the number of points required
 * @param clusters
 * @return
 */
std::vector<Line> extractModels(const std::vector<Cluster> &clusters);




#endif // CLUSTER_H
