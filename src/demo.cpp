/** 
 * Author        : Lysandre M. (lysandre.macke@enpc.fr)
 * Created       : 04-26-2023
 * Last modified : 05-31-2023
 * 
 * Implementation of the T-linkage algorithm for multi-model estimation.
 * Using the Imagine++ library.
 *
 * Inspired by L. Magri and A. Fusiello work 'T-Linkage : a Continuous
 * Relaxation of J-Linkage for Multi-Model Fitting'.
 *
 *
 * This demo program shows an application of the T-Linkage algorithm
 * for line-fitting problems. 2 modes are available :
 *
 * - Multiple lines detection from a randomly generated dataSet
 * - Multiple lines detection from an input image
 *
 * See README.md for more details.
 */

#include <iostream>
#include <chrono>
#include "point.h"
#include "line.h"
#include "cluster.h"
#include "image.h"
using namespace std;


int main(int argc, char **argv) {
    if(argc > 2){
        fprintf(stderr, "usage:\n./tlk [image.png]\n");
        return -1;
    }

    // seed initialization
    srand((unsigned int)time(0));


    int windowWidth, windowHeight;
    PointPool dataSet;


    // load image
    if(1) {
        std::cout << "here" << std::endl;
        cv::Mat image;
//        if(!loadImage(argv[1], image)) {
//            return -1;
//        }
        std::string imageSrc = "../SymmLinkage/input/"; // change to /input for final commit
        imageSrc += DEFAULT_IMAGE;

        if(!loadImage(imageSrc, image)) {
            return -1;
        }

        cv::imshow("Input image", image);

        contourCanny(image);


        windowWidth = image.cols;
        windowHeight = image.rows;

        cv::imshow("tmp", image);

        dataSet = extractPointsFromImage(image);
    }
    else {
        windowWidth = WINDOW_WIDTH;
        windowHeight = WINDOW_HEIGHT;



        // random point generation
//        dataSet = Point::generateRandomDataSetOfSize(N_OUTLIERS);
        dataSet = PointPool::generateRandomDataSetOfSize(N_OUTLIERS);

        // generate models with noise
        for(auto i = 0; i < N_MODELS; i++) {
            auto inliers = Line::randomlyGenerated().generateRandomInliers(N_INLIERS);
            for(auto point : inliers) {
                dataSet.insert(point);
            }
        }

//    //    auto inliers = Line::generateStarModel();
//    //    dataSet.insert(inliers.begin(), inliers.end());
    }
    std::cout << windowWidth << " " << windowHeight << std::endl;
    Imagine::openWindow(windowWidth, windowHeight);

    // cluster generation
    auto modelClusters = Cluster::clusterizePairs(dataSet);
    // extract models from clusterized set
    auto models = extractModels(modelClusters);
    std::cout << "[DEBUG] Extracted " << models.size() << " models" << std::endl;

    /////////////////////////-->
    for(auto model : models) {
        std::cout << model << std::endl;
    }
    //<--///////////////////////
    auto clusters = Cluster::clusterize(dataSet);
    std::cout << "[DEBUG] Starting with " << clusters.size() << " clusters. " << std::endl;

    Cluster::displayClusters(clusters, windowWidth, windowHeight);

    ////////////////////////-->
    /// for debug (remove after)
    ///
    auto tmpWindow = Imagine::openWindow(windowWidth, windowHeight, "models", windowWidth, 10);
    Imagine::setActiveWindow(tmpWindow);

    int i = 0;
    for(auto model : models) {
        model.display(windowWidth, windowHeight);

    }

    //<--/////////////////////////////////////

    // START ALGORITHM
    auto start = chrono::steady_clock::now();

    std::cout << "[DEBUG] Linking clusters, please wait... " << std::endl;

    // link until model is found
    auto linkable = true;
    int linkIndex = 0;
    while(linkable) {
        linkIndex++;
        linkable = link(clusters, dataSet, models);
        std::cout << "linked 2 clusters. Number of clusters : " << clusters.size() << std::endl;
    }
    auto end = chrono::steady_clock::now();

    // display models
    validateBiggestClusters(clusters, dataSet.size());
//    validateNBiggestClusters(1, clusters);

    auto resWindow = Imagine::openWindow(windowWidth, windowHeight, "results", windowWidth, 10);
    Imagine::setActiveWindow(resWindow);
    Cluster::displayValidated(clusters, windowWidth, windowHeight);
    Cluster::displayModels(clusters, windowWidth, windowHeight);
    // for debug : find model tha has the most corresponding points
    auto pf = clusters[1].computePF(models, dataSet);
    auto max = 0;
    auto maxIndex = 0;
    auto index = 0;
    for(auto value : pf) {
        if(value > max) {
            max = value;
            maxIndex = index;
        }
        index++;
    }

    auto tmpWindoww = Imagine::openWindow(windowWidth, windowHeight, "models", windowWidth, 10);
    Imagine::setActiveWindow(tmpWindoww);
    models[maxIndex].display(windowWidth, windowHeight);

    std::cout << "model found has points " << models[maxIndex].a() << " " << models[maxIndex].b() << std::endl;

    std::cout << "[DEBUG] Ending with " << clusters.size() << " clusters after " << linkIndex << " linkages." << std::endl;
    cout << "Time took : " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << std::endl;
    Imagine::endGraphics();

    return 0;
}
