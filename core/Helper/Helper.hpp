#pragma once

#include <random>
#include <chrono>

namespace helper
{
    class CRandom
    {
    public:
        CRandom() : gen(rd()) {}

        double GetRandom();

    private:
        std::random_device rd;
        std::mt19937 gen;
        std::uniform_real_distribution<double> dis{0.0, 1.0};
    };

    double GetRandom();
}
