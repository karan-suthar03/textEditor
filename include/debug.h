#pragma once
#include <cstdio>

#ifndef NDEBUG
#  define LOG(...) std::printf(__VA_ARGS__)
#else
#  define LOG(...) ((void)0)
#endif
