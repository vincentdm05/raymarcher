#pragma once

#include "Common.hpp"

#include "Hitable.hpp"
#include "Math.hpp"

class Material;

class Box : public Hitable
{
private:
	Vec3 dim;

	Box() {}

public:
	Box(const Transform &_transform, const Vec3 &_dimensions, const Material &_material);

	virtual bool evaluateSDF(const Vec3 &point, Real epsilon, HitRecord &rec) const;
};

Box::Box(const Transform &_transform, const Vec3 &_dimensions, const Material &_material)
: Hitable()
{
	transform = _transform.inverse();
	material = &_material;
	dim = _dimensions;
}

bool Box::evaluateSDF(const Vec3 &point, Real epsilon, HitRecord &rec) const
{
	Vec3 p = transform.apply(point);
	Vec3 diff = abs(p) - dim;

	rec.t = max(diff, Vec3(0, 0, 0)).length() + min(max(diff), 0);

	if (rec.t - epsilon <= 0.0)
	{
		rec.point = point;
		rec.normal = Vec3(0, 1, 0);	// TODO
		rec.material = material;

		return true;
	}

	return false;
}
