/**
 * Author        : Lysandre M. (lysandre.macke@enpc.fr)
 * Created       : 04-27-2023
 * Last modified : 06-15-2023 */

#ifndef CLUSTER_H
#define CLUSTER_H

#include <iostream>
#include <set>
#include <chrono> // remove after testing
#include <omp.h>



#include "line.h"
#include "pointpool.h"
#include "image.h"


/** Represents a cluster of points, which can eventually be view as a model hypothesis.
 *  A cluster is a vector of points from the 2D space. */
class Cluster {
public:
    /** Constructors */
    Cluster();

    Cluster(const Cluster& other);

    Cluster(std::shared_ptr<Point>p);

    Cluster(const std::vector<std::shared_ptr<Point>> &points);

    /** Destructor */
    ~Cluster();

    /** Factory method that generates N/2 clusters from the given data set. */
    static std::vector<Cluster> clusterizePairs(const PointPool &points);

    /** Factory method that generates N singleton clusters from a data set
     *  of size N */
    static std::vector<Cluster> clusterize(const PointPool &points);


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
    std::vector<std::shared_ptr<Point>> points() const;

    /** Adds point to cluster. */
    void addPoint(std::shared_ptr<Point>p);

    /** Adds points contained in the given vector to cluster. */
    void addPoints(std::vector<std::shared_ptr<Point>> points);

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
    static void displayClusters(const std::vector<Cluster> &clusters,
                                int windowWidth,
                                int windowHeight);

    /**
     * Displays the given vectors, automatically assigning each one a color.
     *
     * @param clusters the clusters to be displayed.
     */
    static void displayClustersWithColors(const std::vector<Cluster> &clusters,
                                          int windowWidth,
                                          int windowHeight);

    /**
     * Displays only the clusters that are validated.
     *
     * @param clusters the clusters to be displayed.
     */
    static void displayValidated(const std::vector<Cluster> &clusters,
                                 int windowWidth,
                                 int windowHeight);

    static void displayValidatedOnImage(const std::vector<Cluster> &clusters,
                                        int windowWidth,
                                        int windowHeight,
                                        cv::Mat &image);

    /**
     * @brief displayModels
     * @param clusters
     */
    static void displayModels(const std::vector<Cluster> &clusters,
                              int windowWidth,
                              int windowHeight);

    /**
     * Creates a line for clusters of size 2.
     * @return Line object
     */
    Line extractLineModel();

    /**
     *  Computes and returns the preference function of the cluster,
     *  defined as the vector of PF minimums for each point contained
     *  in the cluster.
     */
    std::vector<double> computePF(
            const std::vector<Line> &models,
            PointPool &dataSet
            );

    /**
     * @return true if all points were validated.
     */
    bool isModel();


private:

    // private methods


    // attributes

    std::vector<std::shared_ptr<Point>> _points;                // vector of points composing the cluster

};

///////////////////////////////////////////////////////////////////////////////////////////


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
        PointPool &dataSet,
        const std::vector<Line> &models
        );

/** (DEPRECATED) Validates the n biggest clusters contained in the given vector. */
void validateNBiggestClusters(unsigned int n, std::vector<Cluster> &clusters);

/** Validates the biggest clusters contained in the given vector.
  * The cluster rejection method is directly inspired from L. Magri and A. Fusillo
  * paper.*/
void validateBiggestClusters(std::vector<Cluster> &clusters, int dataSetSize);

/** Validaes the biggest clusters contained in the given vector according to the dataSet size.
 *  A cluster is valid if its size is > minSize.
 *  Assume that the vector is sorted by descending order.
 *  Return the index of the last validated cluster. */
int validateBiggestClusters_2(std::vector<Cluster> &clusters, int dataSetSize);

/**
 * Validates all clusters that have a size >= than a given threshold.
 * @param clusters
 * @param threshold
 * @return
 */
int validateBiggestClusters_3(std::vector<Cluster> &clusters, int threshold);


/**
 * Extract models from clusters, asserting that cluster contains the number of points required
 * @param clusters
 * @return
 */
std::vector<Line> extractModels(const std::vector<Cluster> &clusters);




#endif // CLUSTER_H
