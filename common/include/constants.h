#pragma once

#include <chrono>
#include <cstdint>
#include <ratio>

template <typename T>
const T PI = T(3.141592653589793238462643383279502884L);

typedef std::chrono::duration<std::int64_t, std::micro> microseconds;
