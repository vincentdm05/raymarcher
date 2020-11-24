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

	virtual Real evaluateSDF(const Vec3 &point) const override;
};

Box::Box(const Transform &_transform, const Vec3 &_dimensions, const Material &_material)
: Hitable()
{
	transform = _transform.inverse();
	material = &_material;
	dim = _dimensions;
}

Real Box::evaluateSDF(const Vec3 &point) const
{
	Vec3 p = transform.apply(point);
	Vec3 diff = (abs(p) - dim) * transform.inverseScale();
	return max(diff, Vec3(0, 0, 0)).length() + min(max(diff), 0);
}
