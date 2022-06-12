#include "rand.h"

Rand::Rand()
{
    std::random_device rd;
    _generator = std::mt19937(rd());
}

uint Rand::operator()()
{
    return _generator();
}

uint Rand::range(uint start, uint end)
{
    return start + std::abs(static_cast<int>((*this)())) % (end + 1);
}