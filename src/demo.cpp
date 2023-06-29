/** 
 * Author        : Lysandre M. (lysandre.macke@enpc.fr)
 * Created       : 04-26-2023
 * Last modified : 06-28-2023
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
    cv::Mat inputImage;


    // load image
    if(1) {
//        if(!loadImage(argv[1], image)) {
//            return -1;
//        }
        std::string imageSrc = "../T-Linkage/input/"; // change to /input for final commit
        imageSrc += DEFAULT_IMAGE;

        if(!loadImage(imageSrc, inputImage)) {
            return -1;
        }

        cv::imshow("Input image", inputImage);
        cv::Mat image = inputImage.clone();
        contourCanny(image);
        cv::imshow("contour", image);

        windowWidth = image.cols;
        windowHeight = image.rows;

        dataSet = extractPointsFromImage(image);

        if(dataSet.size() == 0) {
            fprintf(stderr,
                    "Error : could not generate data set.\n"
                    "Make sure that the FILTER_VALUE and/or the CANNY_THRESHOLD is not too big.");
            return -1;
        }
    }
    else {
        windowWidth = WINDOW_WIDTH;
        windowHeight = WINDOW_HEIGHT;

        // random point generation
        dataSet = PointPool::generateRandomDataSetOfSize(N_OUTLIERS);

        // generate models with noise
        for(auto i = 0; i < N_MODELS; i++) {
            auto inliers = Line::randomlyGenerated().generateRandomInliers(N_INLIERS);
            for(auto point : inliers) {
                dataSet.insert(point);
            }
        }
        if(dataSet.size() == 0) {
            fprintf(stderr,
                    "Error : could not generate data set.\n"
                    "Make sure that the summ of the INLIER and OUTLIER parameters"
                    " is strictly superior than 0.");
            return -1;
        }
    }

    Imagine::openWindow(windowWidth, windowHeight);


    // cluster generation
    auto clusters = Cluster::clusterize(dataSet);
    std::cout << "[DEBUG] Starting with " << clusters.size() << " clusters. " << std::endl;

    Cluster::displayClusters(clusters, windowWidth, windowHeight);

    // extract models from clusterized set
    auto models = Line::drawModels(N_MODELS_TO_DRAW, dataSet);

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
//    validateNBiggestClusters(8, clusters);
    validateBiggestClusters(clusters, dataSet.size());
//    validateBiggestClusters_3(clusters, 9);

    // display models

//    auto resWindow = Imagine::openWindow(windowWidth, windowHeight, "results", windowWidth, 10);
//    Imagine::setActiveWindow(resWindow);
//    Cluster::displayValidated(clusters, windowWidth, windowHeight);

//    Cluster::displayModels(clusters, windowWidth, windowHeight);

    if(1) {
        Cluster::displayValidatedOnImage(clusters, windowWidth, windowHeight, inputImage);
        cv::imshow("results", inputImage);
    }


    std::cout << "[DEBUG] Ending with " << clusters.size() << " clusters after " << linkIndex << " linkages." << std::endl;
    cout << "Time took : " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << std::endl;
    Imagine::endGraphics();

    return 0;
}
