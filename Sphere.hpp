#pragma once

#include "Common.hpp"

#include "Hitable.hpp"

class Material;

class Sphere : public Hitable
{
private:
	Vec3 center;
	Real radius = 1.0;
	const Material *material = nullptr;

public:
	Sphere() {}
	Sphere(const Vec3 &_center, Real _radius, const Material &_material) { center = _center; radius = _radius; material = &_material; }

	virtual bool evaluateSDF(const Vec3 &point, Real epsilon, HitRecord &rec) const;
};

bool Sphere::evaluateSDF(const Vec3 &point, Real epsilon, HitRecord &rec) const
{
	rec.t = (point - center).length() - radius;

	if (rec.t - epsilon <= 0.0)
	{
		rec.point = point;
		rec.normal = (rec.point - center) / radius;
		rec.material = material;

		return true;
	}

	return false;
}
