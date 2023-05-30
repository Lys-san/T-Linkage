/**
 * Author        : Lysandre M. (lysandre.macke@enpc.fr)
 * Created       : 04-28-2023
 * Last modified : 04-28-2023 */

#ifndef MODEL_H
#define MODEL_H

#include "point.h"
#include "settings.h"

#include <ostream>
#include <set>
#include <unordered_set>

/** Superclass for a model. */
class Model {
public:    
    /** Destructor */
    virtual ~Model();
    /** Returns the Minimal Sample Set Size of the model. */
    virtual int mmss();

    /** Computes and return the consensus set of the model according to the given dataSet*/
    virtual std::set<Point> computeConsensusSet(const std::set<Point> &dataSet);
};

#endif // MODEL_H
