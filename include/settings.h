/**
 * Author        : Lysandre M. (lysandre.macke@enpc.fr)
 * Created       : 04-28-2023
 * Last modified : 05-31-2023
 *
 * Global parameters for T-Linkage algorithm. */

#ifndef SETTINGS_H
#define SETTINGS_H

#define TAU           0.005      // works like a threshold for PF computing (works well with TAU=0.005)
#define Z             1          // normalization constant (in fact, we can keep it to 1)
#define SQUARED_SIGMA 0.001      // for random sampling (default : 0.001
#define N_MODELS_TO_DRAW 50

////////////////////////////////////////////////////////////////////

#define COLOR_PACK {Imagine::GREEN,   \
                    Imagine::RED,     \
                    Imagine::BLUE,    \
                    Imagine::ORANGE,  \
                    Imagine::PURPLE,  \
                    Imagine::BLACK,   \
                    Imagine::YELLOW   \
                    } // color pack for clusters display (7 colors)

#define N_COLORS 7

////////////////////////////////////////////////////////////////////
// adjust these parameters for testing with random data set
#define N_MODELS      2    // number of models
#define N_INLIERS     30   // inliers for 1 model
#define N_OUTLIERS    50   // outliers in general data set
#define MAX_NOISE     0.00 // max value added to an inlier to ennoise
                           //              the model (default : 0.005)


///////////////////////////////////////////////////////////////////////////
// adjust these parameters for testing with an input image

#define DEFAULT_IMAGE "shapes.jpg" // image to load from the input folder
#define CANNY_THRESHOLD_1 100 // default : 100
#define CANNy_THRESHOLD_2 3*CANNY_THRESHOLD_1
#define FILTER_VALUE  10        // must be > 1 (the more the value gets the less points will be kept)
                                //     (default : 10)
#define ROUND_VALUE   10000.          // must be > 0.
                                   // The bigger this value gets, the more accurate the data set will be generated.
                                   // The more close to 0 the value gets, the more pixelated the dataset will be generated.

#endif // SETTINGS_H
