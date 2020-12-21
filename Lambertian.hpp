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

	virtual Vec3 shade(const Vec3 &incoming, const HitRecord &hr, const Scene &scene, uint bounces) const;
};

Vec3 Lambertian::shade(const Vec3 &incoming, const HitRecord &hr, const Scene &scene, uint bounces) const
{
	Real epsilon = 0.0001;
	Vec3 normal = hr.hitable->evaluateNormal(hr.point, epsilon);
	Vec3 irradiance;
	uint nSamples = 20;
	for (uint i = 0; i < nSamples; i++)
	{
		Vec3 sampleDirection = normal + sampleUnitSphere();

		Ray ray(hr.point, sampleDirection);
		bool hit = false;
		HitRecord rec;
		Real dist = 0.0;
		for (uint iteration = 0; iteration < nSamples; iteration++)
		{
			Vec3 point = ray.to(dist);
			rec.t = max();
			hit = scene.hit(point, 1e-7, rec);
			dist += rec.t;

			if (hit || dist < rec.t || dist > 10)
				break;
		}

		if (hit)
		{
			if (bounces < 1)
				irradiance += rec.hitable->getMaterial()->shade(sampleDirection, rec, scene, bounces + 1);
		}
		else
		{
			irradiance += scene.background().sample(sampleDirection);
		}
	}
	irradiance /= nSamples;

	return irradiance * albedo;
}
