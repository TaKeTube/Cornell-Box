#include "global.hpp"

thread_local std::mt19937 rng(std::random_device{}());