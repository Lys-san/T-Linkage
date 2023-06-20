#include "image.h"

bool loadImage(const std::string &path, cv::Mat &image) {
    std::cout << "loading image ..." << std::endl;
    image = cv::imread(path);
    if(image.empty()){
        std::cout << "error loading " << path << std::endl;
        return false;
    }

    // resize if too large
    if(image.cols > IMG_MAX_WIDTH || image.rows > IMG_MAX_HEIGHT) {
        double factor;
        if(image.cols > image.rows) {
            factor = static_cast<double>(IMG_MAX_WIDTH)/image.cols;
        }
        else {
            factor = static_cast<double>(IMG_MAX_HEIGHT)/image.rows;
        }
        auto dims = cv::Size(image.cols * factor, image.rows*factor);
        cv::resize(image, image, dims, cv::INTER_LINEAR);
    }
    return true;
}


void contour(cv::Mat &image) {
    cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
    cv::GaussianBlur(image, image, cv::Size(7, 7), 0);

    cv::Mat sobelX, sobelY;
    cv::Mat absSobelX, absSobelY;

    cv::Sobel(image, sobelX, CV_16S, 1, 0);
    cv::Sobel(image, sobelY, CV_16S, 0, 1);

    cv::convertScaleAbs(sobelX, absSobelX);
    cv::convertScaleAbs(sobelY, absSobelY);

    cv::addWeighted(absSobelX, 0.5, absSobelY, 0.5, 0, image);
}

unsigned char getAveragePixelValueFrom(const cv::Mat &image) {
    if(image.channels() == 3) {
        cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);
    }

    auto sum = 0.;
    for(int i = 0; i < image.rows; i++) {
        for(int j = 0; j < image.cols; j++) {
            sum += image.at<uchar>(i, j);
        }
    }
    return sum / (image.rows * image.cols);
}

PointPool extractPointsFromImage(const cv::Mat &image) {
    PointPool points;

    auto threshold = getAveragePixelValueFrom(image);

    for(auto i = 0; i < image.rows; i ++) {
        for(auto j = 0; j < image.cols; j++) {
            if(image.at<uchar>(i, j) > 3*threshold) {
                auto x = j/static_cast<double>(image.cols);
                auto y = i/static_cast<double>(image.rows);

                // round to limit the number of points that we save
                x = std::round(x * ROUND_VALUE)/ROUND_VALUE;
                y = std::round(y * ROUND_VALUE)/ROUND_VALUE;

                Point p =  Point(x, y);
                points.insert(p, FILTER_VALUE);
            }
        }
    }
    return points;
}

cv::Mat dog(const cv::Mat &image, int k1, int k2) {
    cv::Mat g1; // first blurred image
    cv::Mat g2; // second blurred image

    cv::GaussianBlur(image, g1, cv::Size(k1, k1), 0);
    cv::GaussianBlur(image, g2, cv::Size(k2, k2), 0);

    return g1 - g2;
}

std::vector<cv::Mat> successiveDoG(const cv::Mat &image, int n) {
    std::vector<cv::Mat> dogs;

    for(struct {int i, j;} loop = {1, 0}; loop.j < n; loop.i+=2, loop.j++) {
        std::cout << "kernel of size " << loop.i << std::endl;
        dogs.emplace_back(dog(image, loop.i, loop.i+2));
    }

    return dogs;
}
