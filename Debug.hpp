#pragma once

#include "Common.hpp"

#include <ostream>
#include <stdlib.h>

template <typename ... Args>
void toCerr(Args&&... args)
{
	(void)(int[]){0, (void(std::cerr << std::forward<Args>(args)), 0)...};
	std::cerr << std::endl;
}	

#define raymAssert(cond, ...) \
	if (!(cond)) \
	{ \
		toCerr("Assertion failed: (", #cond, "), (", __VA_ARGS__, "), function ", __func__, ", file ", __FILE__, ", line ", __LINE__, "."); \
		abort(); \
	}
