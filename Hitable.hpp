#pragma once

#include "Common.hpp"

#include "Ray.hpp"
#include "Transform.hpp"
#include "Vec3.hpp"

class Hitable;
class Material;

struct HitRecord
{
	Real t = 0.0;
	Vec3 point;
	const Hitable *hitable = nullptr;
};

class Hitable
{
protected:
	Transform transform;
	const Material *material = nullptr;

public:
	virtual ~Hitable() {}

	const Material *getMaterial() const { return material; }

	virtual bool hit(const Vec3 &point, Real epsilon, HitRecord &recOut) const;
	virtual Real evaluateSDF(const Vec3 &point) const = 0;
	virtual Vec3 evaluateNormal(const Vec3 &point, Real epsilon) const;
};

bool Hitable::hit(const Vec3 &point, Real epsilon, HitRecord &rec) const
{
	rec.t = evaluateSDF(point);

	if (rec.t - epsilon <= 0.0)
	{
		rec.point = point;
		rec.hitable = this;

		return true;
	}

	return false;
}

Vec3 Hitable::evaluateNormal(const Vec3 &point, Real epsilon) const
{
	Vec3 normal;
	normal.x = evaluateSDF(point + Vec3(epsilon, 0, 0)) - evaluateSDF(point - Vec3(epsilon, 0, 0));
	normal.y = evaluateSDF(point + Vec3(0, epsilon, 0)) - evaluateSDF(point - Vec3(0, epsilon, 0));
	normal.z = evaluateSDF(point + Vec3(0, 0, epsilon)) - evaluateSDF(point - Vec3(0, 0, epsilon));
	return normalize(normal);
}
