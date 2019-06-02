#pragma once

#include "Common.hpp"

#include "Background.hpp"
#include "Material.hpp"
#include "Math.hpp"
#include "Sampling.hpp"
#include "Scene.hpp"

class Lambertian : public Material
{
private:
	Vec3 albedo;

public:
	Lambertian() { albedo = Vec3(0.5, 0.5, 0.5); }
	Lambertian(const Vec3 &_albedo) { albedo = _albedo; }

	virtual Vec3 shade(const HitRecord &hr, const Scene &scene) const;
};

Vec3 Lambertian::shade(const HitRecord &hr, const Scene &scene) const
{
	Real epsilon = 0.0001;
	Vec3 normal = hr.hitable->evaluateNormal(hr.point, epsilon);
	Vec3 irradiance;
	uint nSamples = 50;
	for (uint i = 0; i < nSamples; i++)
	{
		Vec3 sampleDirection = normal + sampleUnitSphere();
		irradiance += scene.background().sample(sampleDirection);
	}
	irradiance /= nSamples;

	return irradiance * albedo;
}
