#ifndef INC_RAND_H
#define INC_RAND_H

#include <random>


typedef unsigned int uint;

class Rand
{
public:
    Rand();
    uint operator()();
    uint range(uint start = 0, uint end = 9);

private:
    std::mt19937 _generator;
};


#endif //INC_RAND_H