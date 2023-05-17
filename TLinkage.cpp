/** 
 * Author        : Lysandre M. (lysandre.macke@enpc.fr)
 * Created       : 04-26-2023
 * Last modified : 05-17-2023
 * 
 * Implementation of the T-linkage algorithm for multi-model estimation.
 * Using the Imagine++ library.
 * 
 * Inspired by L. Magri and A. Fusiello work 'T-Linkage : a Contiuous
 * Relaxation of J-Linkage for Multi-Model Fitting'. */

#include <iostream>
#include <chrono>
#include "point.h"
#include "line.h"
#include "cluster.h"
using namespace std;


int main() {
    Imagine::openWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

    // seed initialization
    srand((unsigned int)time(0));

    // random point generation
    auto dataSet = Point::generateRandomDataSetOfSize(N_OUTLIERS);

    // models with noise
    for(auto i = 0; i < N_MODELS; i++) {
        auto inliers = Line::randomlyGenerated().generateRandomInliers(N_INLIERS);
        dataSet.insert(inliers.begin(), inliers.end());
    }

//    auto inliers = Line::generateStarModel();
//    dataSet.insert(inliers.begin(), inliers.end());

    // cluster generation
    auto modelClusters = Cluster::clusterizePairs(dataSet);
    // extract models from clusterized set
    auto models = extractModels(modelClusters);
    std::cout << "[DEBUG] Extracted " << models.size() << " models" << std::endl;
    auto clusters = Cluster::clusterize(dataSet);
//    auto clusters = modelClusters; // uncomment for ladt version of the algorithm

    Cluster::displayClusters(clusters);

    // START ALGORITHM
    auto start = chrono::steady_clock::now();
    // compute PM once
    auto pm = computePM(models, dataSet);
    std::cout << "[DEBUG] Computed " << pm.size() << " preference sets" << std::endl;
    std::cout << "[DEBUG] Each PS considers " << pm[0].size() << " models" << std::endl;

    std::cout << "-----" << std::endl;
    for(auto line : pm) {
        for(auto value : line) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-----" << std::endl;

//    std::cout << "[DEBUG] Linking clusters, please wait... " << std::endl;


//    // link until model is found
//    auto linkable = true;
//    int linkIndex = 0;
//    while(linkable) {
//        linkIndex++;
//        linkable = link(clusters, dataSet, pm, models);
//    }
//    auto end = chrono::steady_clock::now();

//    // display models
//    validateNBiggestClusters(N_MODELS, clusters);
////    validateBiggestClusters_2(clusters, dataSet.size());
////    validateBiggestClusters(clusters);

//    auto resWindow = Imagine::openWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "results", WINDOW_WIDTH, 10);
//    Imagine::setActiveWindow(resWindow);
//    Cluster::displayValidated(clusters);
//    Cluster::displayModels(clusters);

//    std::cout << "[DEBUG] Ending with " << clusters.size() << " clusters after " << linkIndex << " linkages." << std::endl;
//    cout << "Time took : " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << std::endl;
    Imagine::endGraphics();
    return 0;
}
