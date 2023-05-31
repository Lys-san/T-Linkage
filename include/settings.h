/**
 * Author        : Lysandre M. (lysandre.macke@enpc.fr)
 * Created       : 04-28-2023
 * Last modified : 05-31-2023
 *
 * Global parameters for J-Linkage algorithm. */

#ifndef SETTINGS_H
#define SETTINGS_H


#define TAU           0.005      // works like a threshold for PF computing (works well with TAU=0.005)
#define Z             1          // normalization constant (in fact, we can keep it to 1)
#define SQUARED_SIGMA 0.001      // for random sampling
#define MAX_NOISE     0.001      // max value added to an inlier to ennoise the model (default : 0.005)

////////////////////////////////////////////////////////////////////

#define COLOR_PACK {Imagine::GREEN,   \
                    Imagine::RED,     \
                    Imagine::BLUE,    \
                    Imagine::ORANGE,  \
                    Imagine::PURPLE,  \
                    Imagine::BLACK,   \
                    Imagine::YELLOW   \
                    } // color pack for clusters display (7 colors)

# define N_COLORS 7

////////////////////////////////////////////////////////////////////

// adjust these parameters for testing
#define N_MODELS      2   // number of models
#define N_INLIERS     30 // inliers for 1 model
#define N_OUTLIERS    10 // outliers in general data set


#endif // SETTINGS_H
