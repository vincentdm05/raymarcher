#pragma once

#include "Common.hpp"

#include "Ray.hpp"
#include "Vec3.hpp"

class Material;

struct HitRecord
{
	Real t = 0.0;
	Vec3 normal;
	Vec3 point;
	const Material *material = nullptr;
};

class Hitable
{
public:
	virtual ~Hitable() {}

	virtual bool evaluateSDF(const Vec3 &point, Real epsilon, HitRecord &recOut) const = 0;
};