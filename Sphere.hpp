#pragma once

#include "Common.hpp"

#include "Hitable.hpp"

class Material;

class Sphere : public Hitable
{
private:
	Sphere() {}

public:
	Sphere(const Vec3 &_center, Real _radius, const Material &_material);

	virtual Real evaluateSDF(const Vec3 &point) const override;
};

Sphere::Sphere(const Vec3 &_center, Real _radius, const Material &_material)
: Hitable()
{
	transform.setTranslation(_center);
	transform.setScale(_radius);
	material = &_material;
}

Real Sphere::evaluateSDF(const Vec3 &point) const
{
	return (point - transform.translation()).length() - transform.scale();
}
