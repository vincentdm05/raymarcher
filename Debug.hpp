#pragma once

#include "Common.hpp"

#include <iostream>
#include <stdlib.h>

template <typename ... Args>
void toCerr(Args&&... args)
{
	(void)(int[]){0, (void(std::cerr << std::forward<Args>(args)), 0)...};
	std::cerr << std::endl;
}	

#ifdef NDEBUG

#define raymAssert(...)
#define raymAssertEqual(...)
#define raymAssertEqualWithTolerance(...)

#else	// NDEBUG

#define raymAssert(cond, ...) \
	if (!(cond)) \
	{ \
		toCerr("Assertion failed: (", #cond, "), (", __VA_ARGS__, "), function ", __func__, ", file ", __FILE__, ", line ", __LINE__, "."); \
		abort(); \
	}

#define raymAssertEqual(left, right) \
	if (!(left == right)) \
	{ \
		toCerr("Assertion failed: ", #left, " does not equal ", #right, " (", left, " != ", right, "), function ", __func__, ", file ", __FILE__, ", line ", __LINE__, "."); \
		abort(); \
	}

#define raymAssertEqualWithTolerance(left, right, epsilon) \
	if (!closeEnough(left, right, epsilon)) \
	{ \
		toCerr("Assertion failed: ", #left, " does not equal ", #right, " (", left, " != ", right, "), function ", __func__, ", file ", __FILE__, ", line ", __LINE__, "."); \
		abort(); \
	}

#endif	// NDEBUG
