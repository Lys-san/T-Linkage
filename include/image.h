/**
 * Author        : Lysandre M. (lysandre.macke@enpc.fr)
 * Created       : 06-05-2023
 * Last modified : 06-09-2023 */

#ifndef IMAGE_H
#define IMAGE_H

#include "pointpool.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>

#define IMG_MAX_WIDTH  480
#define IMG_MAX_HEIGHT 320

/**
 * Loads and returns an image from a given path string.
 *
 * @param path path to image
 * @return either the image was loaded or not.
 */
bool loadImage(const std::string &path, cv::Mat &image);

/**
 * Applies a sobel filter to the given image.
 *
 * @param img
 */
void contour(cv::Mat &image);

/**
 * Returns the average greyscale value of a given image.
 *
 * @param image
 * @return
 */
unsigned char getAveragePixelValueFrom(const cv::Mat &image);

/**
 * Extracts points from a given image.
 *
 * @param img
 * @return
 */
PointPool extractPointsFromImage(const cv::Mat &image);

/**
 * Returns the DoG (Difference of Gaussians) for the given image.
 *
 * @param image
 * @param k1 the size of the kernel for the fist gaussian blur.
 * @param k2 the size of the kernel for the second gaussian blur.
 *
 * @return the difference of gaussians
 */
cv::Mat dog(const cv::Mat &image, int k1, int k2);

/**
 * Return the successive DoGs of the input image.
 *
 * @param image
 * @param n the number of DoGs to compute
 * @return a vector of DoGs (cv::Mat)
 */
std::vector<cv::Mat> successiveDoG(const cv::Mat &image, int n);

#endif // IMAGE_H
