#include "cluster.h"

Cluster::Cluster() {}


Cluster::Cluster(const Cluster& other) {
    for(auto point : other.points()) {
        _points.emplace_back(point);
    }
}

Cluster::Cluster(Point p) {
    _points.emplace_back(p);
}



Cluster::Cluster(const std::set<Point> &points) {
    for(auto point : points) {
        _points.emplace_back(point);
    }
}

Cluster::~Cluster() {}


std::vector<Cluster> Cluster::clusterizePairs(const std::set<Point> &points) {
    auto dataSet = points;         // copy data set points
    std::vector<Cluster> clusters; // our set of clusters

    // building clusters until no more points in data set
    while(dataSet.size() > 0) {
        std::set<Point> clusterPoints;         // will store points from our cluster

        // retrieve a first random point from data set
        auto i = std::rand() % dataSet.size();
        auto it = dataSet.begin();
        std::advance(it, i);
        Point p = *it;
        clusterPoints.emplace(p);
        dataSet.erase(it);


        for(int i = 0; i < std::min(1UL, dataSet.size()); i++) { // change 1UL value if want to make bigger clusters
            // computing probability according to last selected point
            std::discrete_distribution<> d = p.computeProbabilitiesFor(dataSet);
            std::random_device rd;
            std::mt19937 gen(rd());

            int point_index = d(gen);

            // what if second point is chosen uniformly ?
//            int point_index = std::rand() % dataSet.size();

            it = dataSet.begin();
            std::advance(it, point_index);
            p = *it;

            clusterPoints.emplace(p);
            dataSet.erase(it);
        }

        auto cluster = Cluster(clusterPoints);
        clusters.emplace_back(cluster);
    }
    std::cout<< "[DEBUG] End of random sampling. Generated "
             << clusters.size()  << " models for total data of size  : " << points.size() << std::endl;
    return clusters;
}

std::vector<Cluster> Cluster::clusterize(const std::set<Point> &points) {
    std::vector<Cluster> clusters;

    for(auto point : points) {
        clusters.emplace_back(Cluster(point));
    }

    return clusters;

}

std::ostream &operator<<(std::ostream &out, Cluster &cluster) {
    for(auto point : cluster.points()) {
        std::cout << point << std::endl;
    }
    return out;
}

std::vector<Point> Cluster::points() const {
    return _points;
}


void Cluster::addPoint(Point p) {
    _points.emplace_back(p);
}

void Cluster::addPoints(std::vector<Point> points) {
    _points.insert(_points.end(), points.begin(), points.end());
}

void Cluster::validate() {
    for(Point &point : _points) {
        point.accept();
    }
}

void Cluster::invalidate() {
    for(auto point : _points) {
        point.reject();
    }
}

int Cluster::size() {
    return _points.size();
}

void Cluster::displayClusters(const std::vector<Cluster> &clusters) {
    for(auto cluster:clusters) {
        for(auto point : cluster.points()) {
            point.display();
        }
    }
}

void Cluster::displayClustersWithColors(const std::vector<Cluster> &clusters) {
    Imagine::Color cols[] = COLOR_PACK;
    int i = 0;

    for(auto cluster:clusters) {
        auto col = cols[i % N_COLORS];
        for(auto point : cluster.points()) {
            point.display(col);
        }
        i++;
    }
}

void Cluster::displayValidated(const std::vector<Cluster> &clusters) {
    for(auto cluster : clusters) {
        if(cluster.isModel()) {
            std::cout << "[DEBUG] VALID MODEL of size " << cluster.size() << std::endl;
            for(auto point : cluster.points()) {
                    point.display();

            }
        }
    }
}

void Cluster::displayModels(const std::vector<Cluster> &clusters) {
    for(auto cluster : clusters) {
        if(cluster.isModel()) {
            auto model = Line::leastSquares(cluster._points);
            model.display();
        }
    }
}

Line Cluster::extractLineModel() {
    assert(size() == 2);

    return Line(_points[0], _points[1]);
}

Point Cluster::extractPointModel() {
    return *points().begin();
}

bool Cluster::operator<(const Cluster &other) const {
    return this->_points.size() < other._points.size();
}

bool Cluster::operator==(const Cluster &other) const {
    return this->points() == other.points();
}

std::vector<double> Cluster::computePF(const std::vector<Line> &models) {
    assert(size() > 0);

    std::vector<double> pf;

    if(size() == 1) {
        return computePreferenceFunctionFor(_points[0], models);
    }

    // find min PF value for each model
    for(auto model : models) {
        auto min = model.PFValue(_points.at(0)); // temporary min value
        // compare for each point to find min
        for(auto point : _points) {
            auto tmp = model.PFValue(point);
            if(tmp < min) {
                min = tmp;
            }
        }
        pf.emplace_back(min);
    }
    return pf;
}

bool Cluster::isModel() {
    for(auto point : _points) {
        if(!point.isInlier()) {
            return false;
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////


double tanimoto(std::vector<double> a, std::vector<double> b) {
    assert(a.size() == b.size());

    double a_squaredNorm = std::inner_product(a.begin(), a.end(), a.begin(), 0.0L);
    double b_squaredNorm = std::inner_product(b.begin(), b.end(), b.begin(), 0.0L);

    double ab_innerProduct = std::inner_product(a.begin(), a.end(), b.begin(), 0.0L);

    return 1 - ab_innerProduct/(a_squaredNorm + b_squaredNorm - ab_innerProduct);
}

bool link(std::vector<Cluster> &clusters, std::set<Point> &dataSet, const std::vector<Line> &models) {
    int iFirst     = 0;     // index of first cluster to link
    int iSecond    = 0;     // index of second cluster to link
    double minDist = 1.;    // min. distance between clusters PS (default : 1.)
    bool linkable  = false; // do we apply link operation on clusters or not
    int i          = 0;     // first loop index
    int j          = 0;     // second loop index

    // find closest clusters according to jaccard distance
    for(auto c1 : clusters) {
        j = 0;
        auto pf1 = c1.computePF(models);
        // for each other buffer
        for(auto c2 : clusters) {
            auto pf2 = c2.computePF(models);
            // compare indexes so we don't try to merge a cluster with itself
            double dist = i != j ? tanimoto(pf1, pf2) : 1.;

            if(dist < minDist) {
                minDist = dist;
                iFirst = i;
                iSecond = j;
                linkable = true;
            }
            j++;
        }
        i++;
    }

    // merge
    if(linkable) {
        // the second cluster should be the smallest for faster merging
        if(clusters[iFirst].size() < clusters[iSecond].size()) {
            auto tmp = iFirst;
            iFirst = iSecond;
            iSecond = tmp;
        }

        Cluster &mergingCluster = clusters[iFirst];

        mergingCluster.addPoints(clusters[iSecond].points());

        // erase second buffer
        clusters.erase(clusters.begin() + iSecond);
    }
    return linkable;
}

void validateNBiggestClusters(unsigned int n, std::vector<Cluster> &clusters) {
    assert(clusters.size() > 0);

    std::sort(clusters.begin(), clusters.end());
    std::reverse(clusters.begin(), clusters.end());
    for(int i = 0; i < n; i++) {
        clusters[i].validate();
    }
}

void validateBiggestClusters(std::vector<Cluster> &clusters) {    
    assert(clusters.size() > 0);

    std::sort(clusters.begin(), clusters.end());
    std::reverse(clusters.begin(), clusters.end());

    // for debug, remove after (or maybe not...?)
    std::cout << "[DEBUG] Final cluster sizes : " << std::endl;
    for(auto cluster : clusters) {
        std::cout << cluster.size() << " ";
    }
    std::cout << std::endl;

    std::vector<int> sizes;

    for(auto cluster : clusters) {
        sizes.emplace_back(cluster.size());
    }
    // add dummy cluster to cater for outliers-free case
    sizes.emplace_back(1);

    std::vector<int> diff;
    std::adjacent_difference(sizes.begin(), sizes.end(), sizes.begin());
    std::transform(sizes.begin(), sizes.end(), sizes.begin(), [](int s) {return s > 0 ? s : -s;});

    auto it = std::max_element(sizes.begin() + 1, sizes.end());
    auto index = std::distance(sizes.begin(), it);

    for(int i = 0; i < index; i++) {
        clusters[i].validate();
    }
}

void validateBiggestClusters_2(std::vector<Cluster> &clusters, int dataSetSize) {
    assert(clusters.size() > 0);

    int minSize = 0.10 * dataSetSize;
    std::cout << minSize << std::endl;

    for(Cluster &cluster : clusters) {
        if(cluster.size() > minSize) {
            cluster.validate();
        }
    }
}

std::vector<Line> extractModels(const std::vector<Cluster> &clusters) {
    std::vector<Line> models;
    for(auto cluster : clusters) {
        if(cluster.size() == 2) {
            models.emplace_back(cluster.extractLineModel());
        }
    }
    return models;
}
