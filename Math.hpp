#pragma once

#include "Common.hpp"

#include "Vec3.hpp"

inline Real min(Real a, Real b)
{
	return a < b ? a : b;
}

inline Real min(const Vec3 &v)
{
	return min(v.x, min(v.y, v.z));
}

inline Vec3 min(const Vec3 &a, const Vec3 &b)
{
	return Vec3(a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y, a.z < b.z ? a.z : b.z);
}

inline Real max(Real a, Real b)
{
	return a > b ? a : b;
}

inline Real max(const Vec3 &v)
{
	return max(v.x, max(v.y, v.z));
}

inline Vec3 max(const Vec3 &a, const Vec3 &b)
{
	return Vec3(a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y, a.z > b.z ? a.z : b.z);
}

inline Vec3 abs(const Vec3 &v)
{
	return Vec3(abs(v.x), abs(v.y), abs(v.z));
}

inline Real clamp(Real a, Real lower, Real upper)
{
	return min(upper, max(lower, a));
}

inline Vec3 lerp(const Vec3 &a, const Vec3 &b, Real t)
{
	t = clamp(t, 0, 1);
	return (1 - t) * a + t * b;
}
