#pragma once

#include "Common.hpp"

#include "Hitable.hpp"

class Material;

class Sphere : public Hitable
{
private:

public:
	Sphere() {}
	Sphere(const Vec3 &_center, Real _radius, const Material &_material);

	virtual bool evaluateSDF(const Vec3 &point, Real epsilon, HitRecord &rec) const;
};

Sphere::Sphere(const Vec3 &_center, Real _radius, const Material &_material)
: Hitable()
{
	transform.setTranslation(_center);
	transform.setScale(_radius);
	material = &_material;
}

bool Sphere::evaluateSDF(const Vec3 &point, Real epsilon, HitRecord &rec) const
{
	rec.t = (point - transform.translation()).length() - transform.scale();

	if (rec.t - epsilon <= 0.0)
	{
		rec.point = point;
		rec.normal = (rec.point - transform.translation()) / transform.scale();
		rec.material = material;

		return true;
	}

	return false;
}
