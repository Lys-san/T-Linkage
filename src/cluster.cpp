#include "cluster.h"

Cluster::Cluster() {}


Cluster::Cluster(const Cluster& other) {
    for(auto point : other.points()) {
        _points.emplace_back(point);
    }
}

Cluster::Cluster(std::shared_ptr<Point>p) {
    _points.emplace_back(p);
}

Cluster::Cluster(const std::vector<std::shared_ptr<Point>> &points) {
    for(auto point : points) {
        _points.emplace_back(point);
    }
}

Cluster::~Cluster() {}


std::vector<Cluster> Cluster::clusterizePairs(const PointPool &points) {
    std::vector<Cluster> clusters; // our set of clusters

    int index = 0;

    std::set<int> indexes; // store indexes that were already drawn

    // building clusters until no more points in data set
    while(index++  < N_MODELS_TO_DRAW) {
        std::vector<std::shared_ptr<Point>> clusterPoints;         // will store points from our cluster

        // retrieve a first random point from data set
        auto i = std::rand() % points.size();

        while(indexes.find(i) != indexes.end()) {
            i = std::rand() % points.size();
        }
        indexes.insert(i);
        auto p = points.at(i);
        clusterPoints.emplace_back(p);


        for(int i = 0; i < std::min(1UL, points.size()); i++) { // change 1UL value if want to make bigger clusters
            // computing probability according to last selected point
            std::discrete_distribution<> d = p->computeProbabilitiesFor(points.points());
            std::random_device rd;
            std::mt19937 gen(rd());

            int point_index = d(gen);

            // what if second point is chosen uniformly ?

//            int point_index = std::rand() % points.size();

            while(indexes.find(point_index) != indexes.end()) {
                point_index = d(gen);
            }
            indexes.insert(point_index);
            clusterPoints.emplace_back(points.at(point_index));
        }

        auto cluster = Cluster(clusterPoints);
        clusters.emplace_back(cluster);
    }
    std::cout<< "[DEBUG] End of random sampling. Generated "
             << clusters.size()  << " models for total data of size  : " << points.size() << std::endl;
    return clusters;
}



std::vector<Cluster> Cluster::clusterize(const PointPool &points) {
    std::vector<Cluster> clusters;

//    for(auto point : points) {
//        clusters.emplace_back(Cluster(point));
//    }
    for(auto i = 0; i < points.size(); i++) {
        clusters.emplace_back(Cluster(points[i]));
    }

    return clusters;

}

std::ostream &operator<<(std::ostream &out, Cluster &cluster) {
    for(auto point : cluster.points()) {
        std::cout << point << std::endl;
    }
    return out;
}

std::vector<std::shared_ptr<Point>> Cluster::points() const {
    return _points;
}


void Cluster::addPoint(std::shared_ptr<Point>p) {
    _points.emplace_back(p);
}

void Cluster::addPoints(std::vector<std::shared_ptr<Point>> points) {
    _points.insert(_points.end(), points.begin(), points.end());
}

void Cluster::validate() {
    for(std::shared_ptr<Point>point : _points) {
        point->accept();
    }
}

void Cluster::invalidate() {
    for(std::shared_ptr<Point>point : _points) {
        point->reject();
    }
}

int Cluster::size() {
    return _points.size();
}

void Cluster::displayClusters(const std::vector<Cluster> &clusters, int windowWidth, int windowHeight) {
    for(auto cluster:clusters) {
        for(auto point : cluster.points()) {
            point->display(windowWidth, windowHeight);
        }
    }
}

void Cluster::displayClustersWithColors(const std::vector<Cluster> &clusters, int windowWidth, int windowHeight) {
    Imagine::Color cols[] = COLOR_PACK;
    int i = 0;

    for(auto cluster:clusters) {
        auto col = cols[i % N_COLORS];
        for(auto point : cluster.points()) {
            point->display(col, windowWidth, windowHeight);
        }
        i++;
    }
}

void Cluster::displayValidated(const std::vector<Cluster> &clusters, int windowWidth, int windowHeight) {
    for(auto cluster : clusters) {
        if(cluster.isModel()) {
            std::cout << "[DEBUG] VALID MODEL of size " << cluster.size() << std::endl;

            for(auto point : cluster.points()) {
                    point->display(windowWidth, windowHeight);
            }
        }
    }
}

void Cluster::displayValidatedOnImage(const std::vector<Cluster> &clusters, int windowWidth, int windowHeight, cv::Mat &image) {
    for(auto cluster : clusters) {
        if(cluster.isModel()) {
            std::cout << "[DEBUG] VALID MODEL of size " << cluster.size() << std::endl;
            auto line = Line::leastSquares(cluster._points);
            drawLineOnImage(image, line);
        }
    }
}

void Cluster::displayModels(const std::vector<Cluster> &clusters, int windowWidth, int windowHeight) {
    for(auto cluster : clusters) {
        if(cluster.isModel()) {
            auto model = Line::leastSquares(cluster._points);
            model.display(windowWidth, windowHeight);
        }
    }
}


Line Cluster::extractLineModel() {
    assert(size() == 2);

    return Line(*_points[0], *_points[1]);
}



bool Cluster::operator<(const Cluster &other) const {
    return this->_points.size() < other._points.size();
}

bool Cluster::operator==(const Cluster &other) const {
    return this->points() == other.points();
}

std::vector<double> Cluster::computePF(const std::vector<Line> &models, PointPool &dataSet) {
    assert(size() > 0);

    std::vector<double> pf;

//    if(size() == 1) {
//        return computePreferenceFunctionFor(_points[0], models);
//    }

    // find min PF value for each model
    for(auto model : models) {
        auto min = model.PFValue(*_points.at(0)); // temporary min value
        // compare for each point to find min
        for(auto point : _points) {
            auto tmp = model.PFValue(*point);
            if(tmp < min) {
                min = tmp;
                if(tmp == 0.) {
                    break; // no need to do more computation !
                }
            }
        }
        pf.emplace_back(min);
    }
    return pf;
}

std::vector<double> Cluster::computePF(const std::vector<Circle> &models, PointPool &dataSet) {
    assert(size() > 0);

    std::vector<double> pf;

//    if(size() == 1) {
//        return computePreferenceFunctionFor(_points[0], models);
//    }

    // find min PF value for each model
    for(auto model : models) {
        auto min = model.PFValue(*_points.at(0)); // temporary min value
        // compare for each point to find min
        for(auto point : _points) {
            auto tmp = model.PFValue(*point);
            if(tmp < min) {
                min = tmp;
                if(tmp == 0.) {
                    break; // no need to do more computation !
                }
            }
        }
        pf.emplace_back(min);
    }
    return pf;
}

bool Cluster::isModel() {
    for(auto point : _points) {
        if(!point->isInlier()) {
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

bool link(std::vector<Cluster> &clusters, PointPool &dataSet, const std::vector<Line> &models) {
    int iFirst     = 0;     // index of first cluster to link
    int iSecond    = 0;     // index of second cluster to link
    double minDist = 1.;    // min. distance between clusters PS (default : 1.)
    bool linkable  = false; // do we apply link operation on clusters or not
    int i          = 0;     // first loop index
    int j          = 0;     // second loop index


    // find closest clusters according to jaccard distance
    for(auto c1 : clusters) {
        j = 0;
        auto pf1 = c1.computePF(models, dataSet);
        // for each other buffer
        for(auto c2 : clusters) {
            auto pf2 = c2.computePF(models, dataSet);
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

bool link(std::vector<Cluster> &clusters, PointPool &dataSet, const std::vector<Circle> &models) {
    int iFirst     = 0;     // index of first cluster to link
    int iSecond    = 0;     // index of second cluster to link
    double minDist = 1.;    // min. distance between clusters PS (default : 1.)
    bool linkable  = false; // do we apply link operation on clusters or not
    int i          = 0;     // first loop index
    int j          = 0;     // second loop index


    // find closest clusters according to jaccard distance
    for(auto c1 : clusters) {
        j = 0;
        auto pf1 = c1.computePF(models, dataSet);
        // for each other buffer
        for(auto c2 : clusters) {
            auto pf2 = c2.computePF(models, dataSet);
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

void validateBiggestClusters(std::vector<Cluster> &clusters, int dataSetSize) {
    assert(clusters.size() > 0);


    auto tmp = clusters;

    // sort by size
    std::sort(clusters.begin(), clusters.end());
    std::reverse(clusters.begin(), clusters.end());

    // big enough clusters are validated by default
    auto start = validateBiggestClusters_2(clusters, dataSetSize);


    // for debug, remove after (or maybe not...?)
    std::cout << "[DEBUG] Final cluster sizes : " << std::endl;
    for(auto cluster : clusters) {
        std::cout << cluster.size() << " ";
    }
    std::cout << std::endl;

    std::vector<int> sizes;

    // compare sizes of remaining clusters
    for(int i = start; i < clusters.size(); i++) {
        sizes.emplace_back(clusters[i].size());
    }

    // add dummy cluster to cater for outliers-free case
    sizes.emplace_back(1);

    std::vector<int> diff;
    std::adjacent_difference(sizes.begin(), sizes.end(), sizes.begin());
    std::transform(sizes.begin(), sizes.end(), sizes.begin(), [](int s) {return s > 0 ? s : -s;});

    auto it = std::max_element(sizes.begin() + 1, sizes.end());
    if(*it >= 4) {
        auto index = std::distance(sizes.begin(), it);

        for(int i = start; i < start + index; i++) {
            clusters[i].validate();
        }
    }
}

int validateBiggestClusters_2(std::vector<Cluster> &clusters, int dataSetSize) {
    assert(clusters.size() > 0);

    int minSize = 0.1 * dataSetSize;
    std::cout << minSize << std::endl;

    int index = 0;

    for(Cluster &cluster : clusters) {
        if(cluster.size() > minSize) {
            cluster.validate();
        }
        else {
            break;
        }
        index++;
    }
    return index;
}

int validateBiggestClusters_3(std::vector<Cluster> &clusters, int threshold) {
    assert(clusters.size() > 0);

    // sort by size
    std::sort(clusters.begin(), clusters.end());
    std::reverse(clusters.begin(), clusters.end());

    int index = 0;

    for(Cluster &cluster : clusters) {
        if(cluster.size() >= threshold) {
            cluster.validate();
        }
        else {
            break;
        }
        index++;
    }
    return index;
}

std::vector<Line> extractModels(const std::vector<Cluster> &clusters) {
    std::set<Line> models;
    for(auto cluster : clusters) {
        if(cluster.size() == 2) {
            models.insert(cluster.extractLineModel());
        }
    }
    std::vector<Line> returnModels(models.begin(), models.end());
    return returnModels;
}
