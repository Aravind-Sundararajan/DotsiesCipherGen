#ifndef BASE_H_
#define BASE_H_

// C++ Standard Library
#include <bitset>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <functional>
#include <utility>
#include <memory>
#include <stdexcept>

// I/O and formatting
#include <iomanip>
#include <fstream>
#include <sstream>

// Numeric and math
#include <cstdint>
#include <cmath>
#include <limits>
#include <numeric>

// C Standard Library
#include <cstring>
#include <cstdlib>
#include <cassert>

// Time and random
#include <chrono>
#include <random>

// Conditional includes for ncurses
#ifndef NO_NCURSES
  #if defined(_WIN32) || defined(_WIN64)
    // Windows-specific includes
    // Note: ncurses is often not available on Windows
    // You might need PDCurses or other alternatives
  #else
    // Unix/Linux/macOS systems
    #include <curses.h>
  #endif
#endif

// Namespace aliases for commonly used functionality
namespace chrono = std::chrono;

// Constants
const size_t BITSET_SIZE = 5;

// C++11 compatibility - constexpr template method for power
template<typename T>
T constexpr_pow(T base, unsigned exponent) {
    return exponent == 0 ? 1 : base * constexpr_pow(base, exponent - 1);
}

#endif // BASE_H_
