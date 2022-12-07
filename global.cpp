#include "global.hpp"

const float EPSILON = 0.0001;
thread_local std::mt19937 rng(std::random_device{}());