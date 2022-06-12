#include "set_operations_distribution.h"

typedef unsigned int uint;

SetOperationsDistribution::SetOperationsDistribution(uint a, uint r, uint c)
    :add{a},remove{r},contains{c}{}

uint SetOperationsDistribution::total() const
{
    return add + remove + contains;
}

char SetOperationsDistribution::pickRandom() const
{
    Rand rand;
    double y = (static_cast<double>(total()) / 100.0) * static_cast<double>(rand.range(0, 100));
    uint a = add;
    uint r = add + remove;
    uint c = r + contains;
    char op;
    if(0 <= y && y < a) {op = 'a';}
    else if(a <= y && y < r) {op = 'r';}
    else {op = 'c';}
    //DEBUGLN(y << " >> " << 0 << "-a-" << a << "-r-" << r << "-c-" << c << " --> " << op);
    return op;
}

float SetOperationsDistribution::addPercent() const
{
    return roundf((static_cast<float>(add) / total()) * 100.0 * 100.0) / 100;;
}

float SetOperationsDistribution::removePercent() const
{
    return roundf((static_cast<float>(remove) / total()) * 100.0 * 100.0) / 100;;
}

float SetOperationsDistribution::containsPercent() const
{
    return roundf((static_cast<float>(contains) / total()) * 100.0 * 100.0) / 100;;
}

std::ostream& operator<< (std::ostream &out, const SetOperationsDistribution &sod)
{
    out << "(add: " << sod.addPercent() << "%,"
        << "remove: " << sod.removePercent() << "%,"
        << "contains: " << sod.containsPercent() << "%)";
    return out;
}