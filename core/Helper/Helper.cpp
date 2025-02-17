#include "Helper.hpp"

helper::CRandom randomHelper;

double helper::CRandom::GetRandom()
{
    return dis(gen);
}

double helper::GetRandom()
{
    return randomHelper.GetRandom() + (std::chrono::high_resolution_clock::now().time_since_epoch().count() % 1000) * 1e-9;
}