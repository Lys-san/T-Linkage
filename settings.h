/**
 * Author        : Lysandre M. (lysandre.macke@enpc.fr)
 * Created       : 04-28-2023
 * Last modified : 05-24-2023
 *
 * Global parameters for J-Linkage algorithm. */

#ifndef SETTINGS_H
#define SETTINGS_H


#define TAU           0.001    // works like a threshold for PF computing
#define Z             1          // normalization constant (in fact, we can keep it to 1)
#define SQUARED_SIGMA 0.001      // for random sampling
#define MAX_NOISE     0.00      // max value added to an inlier to ennoise the model (default : 0.01)


#define COLOR_PACK {Imagine::GREEN,   \
                    Imagine::RED,    \
                    Imagine::BLUE,    \
                    Imagine::ORANGE,  \
                    Imagine::PURPLE,  \
                    Imagine::BLACK,     \
                    Imagine::YELLOW   \
                    } // color pack for clusters display (7 colors)

// adjust these parameters for testing
#define N_MODELS      10   // nuber of models
#define N_INLIERS     50 // inliers for 1 model
#define N_OUTLIERS    30 // in general data set



#define N_COLORS 9

#endif // SETTINGS_H
