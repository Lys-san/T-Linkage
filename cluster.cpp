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
             << clusters.size()  << " clusters for total data of size  : " << points.size() << std::endl;
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

    std::vector<double> ps;
    if(size() == 1) {
        return computePreferenceFunctionFor(_points[0], models);
    }

    for(auto i = 0; i < models.size(); i++) {
        auto min = computePreferenceFunctionFor(_points[0], models)[i];
        for(auto point : _points) {
            auto tmp = computePreferenceFunctionFor(point, models)[i];
            if(tmp < min) {
                min = tmp;
            }
        }
        ps.emplace_back(min);
    }

    return ps;
}

std::set<Line> Cluster::makeInter(const std::set<Line> &a, const std::set<Line> &b) {
    std::set<Line> inter;
    for(auto line : a) {
        if(b.find(line) != b.end()) {
            inter.insert(line);
        }
    }
    for(auto line : b) {
        if(a.find(line) != a.end()) {
            inter.insert(line);
        }
    }
    return inter;

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


std::map<Point, std::set<Line>> extractPSfromPM(const std::set<Point> &dataSet, const std::vector<Line> &models, const std::vector<std::vector<bool>> &pm) {
    std::map<Point, std::set<Line>> preferenceSets;

    int pointIndex;
    int modelIndex;

    pointIndex = 0;
    for(auto psLine : pm) { // loop on rows (points)
        modelIndex = 0;
        auto pointIterator = dataSet.begin();
        std::advance(pointIterator, pointIndex);

        // constructing each ps
        std::set<Line> ps;
        auto val = *pointIterator;
        for(auto b : psLine) { // loop on columns (models)

            if(b) {
                auto tmp = models.at(modelIndex);

                ps.insert(models.at(modelIndex));
            }
            modelIndex++;
        }
        pointIndex++;
        preferenceSets.emplace(std::make_pair(*pointIterator, ps));
    }
    return preferenceSets;
}

std::vector<std::vector<double>> computePM(const std::vector<Line> &models, const std::set<Point> dataSet) {
    std::vector<std::vector<double>> pm;

    for(auto point:dataSet) {
        pm.emplace_back(computePreferenceFunctionFor(point, models));
    }
    return pm;
}

std::vector<std::vector<bool>> transposatePM(const std::vector<std::vector<bool>> &pm) {
    std::vector<std::vector<bool>> transposate(pm.begin()->size(), std::vector<bool>(pm.size(), false));

    for(int i = 0; i < pm.size(); i++) {
        for(int j = 0; j < pm.begin()->size(); j++)
        transposate.at(j).at(i) = pm.at(i).at(j);
    }
    return transposate;
}

double jaccard(std::set<Line> a, std::set<Line> b) {
    std::set<Line> u; // union

    double u_size = a.size() + b.size();

    std::set<Line> n; // intersection
    n = Cluster::makeInter(a, b);

    return (u_size - n.size())/u_size;
}

double tanimoto(std::vector<double> a, std::vector<double> b) {
    assert(a.size() == b.size());

    double a_squaredNorm = std::inner_product(a.begin(), a.end(), a.begin(), 0.0L);
    double b_squaredNorm = std::inner_product(b.begin(), b.end(), b.begin(), 0.0L);

    double ab_innerProduct = std::inner_product(a.begin(), a.end(), b.begin(), 0.0L);

    return 1 - ab_innerProduct/(a_squaredNorm + b_squaredNorm - ab_innerProduct);
}

bool link(std::vector<Cluster> &clusters,
          std::set<Point> &dataSet,
          const std::vector<std::vector<double>> &pm,
          const std::vector<Line> &models
          ) {

    int iFirst     = 0;     // index of first cluster to link
    int iSecond    = 0;     // index of second cluster to link
    double minDist = 1.;    // min. distance between clusters PS (default : 1.)
    bool linkable  = false; // do we apply link operation on clusters or not
    int i          = 0;     // first loop index
    int j          = 0;     // second loop index

    std::cout << "[DEBUG] ---------" << std::endl;

    // find closest clusters according to jaccard distance
    for(auto c1 : clusters) {
        j = 0;
        auto pf1 = c1.computePF(models);
        // for each other buffer
        for(auto c2 : clusters) {
            auto pf2 = c2.computePF(models);
            std::cout << "[DEBUG] PFs are size " << pf1.size() << " " << pf2.size() << std::endl;;
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

    if(linkable) {
        // merge clusters
        Cluster &mergingCluster = clusters[iFirst];

        // put second buffer's content into first
        for(auto point : clusters[iSecond].points()) {
            mergingCluster.addPoint(point);
        }
        auto it = std::find(clusters.begin(), clusters.end(), clusters[iSecond]);

        // erase second buffer
        clusters.erase(it);
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

    std::vector<int> sizes;

    for(auto cluster : clusters) {
        sizes.emplace_back(cluster.size());
    }

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


