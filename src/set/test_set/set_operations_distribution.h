#ifndef INC_SET_OPERATIONS_DISTRIBUTION_H
#define INC_SET_OPERATIONS_DISTRIBUTION_H

#include <string>
#include <sstream>
#include <cmath>

#include "../../log.h"
#include "../../rand/rand.h"


struct SetOperationsDistribution
{
    SetOperationsDistribution(uint a = 1, uint r = 1, uint c = 1);

    uint total() const;
    char pickRandom() const;

    float addPercent() const;
    float removePercent() const;
    float containsPercent() const;

    friend std::ostream& operator<< (std::ostream &out, const SetOperationsDistribution &sod);

    uint add;
    uint remove;
    uint contains;
};


typedef const SetOperationsDistribution& SodLinkT;

#endif //INC_SET_OPERATIONS_DISTRIBUTION_H