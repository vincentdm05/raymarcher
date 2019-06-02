#pragma once

#include "Common.hpp"

#include <math.h>
#include <limits>

inline Real min()
{
	return std::numeric_limits<Real>::min();
}

inline Real max()
{
	return std::numeric_limits<Real>::max();
}

inline Real min(Real a, Real b)
{
	return a < b ? a : b;
}

inline Real max(Real a, Real b)
{
	return a > b ? a : b;
}

inline Real clamp(Real a, Real lower, Real upper)
{
	return min(upper, max(lower, a));
}

inline bool closeEnough(Real a, Real b, Real epsilon)
{
	return abs(a - b) < epsilon;
}
