#pragma once

#include "Common.hpp"

#include "Ray.hpp"
#include "Vec3.hpp"

struct HitRecord;
class Scene;

class Material
{
public:
	virtual ~Material() {}

	virtual Vec3 shade(const Vec3 &incoming, const HitRecord &hr, const Scene &scene, uint bounces) const = 0;
};

Real schlick(Real cosine, Real refractionIndex)
{
	// Assuming air is the outside medium
	const Real mediumRefractionIndex = 1;
	Real r0 = (mediumRefractionIndex - refractionIndex) / (mediumRefractionIndex + refractionIndex);
	r0 *= r0;
	return r0 + (1 - r0) * pow(1 - cosine, 5);
}
