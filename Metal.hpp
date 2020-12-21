#pragma once

#include "Common.hpp"

#include "Background.hpp"
#include "Hitable.hpp"
#include "Material.hpp"
#include "Math.hpp"
#include "Sampling.hpp"
#include "Scene.hpp"

class Metal : public Material
{
private:
	Vec3 albedo;
	Real roughness;

public:
	Metal() { albedo = Vec3(0.5, 0.5, 0.5); roughness = 0; }
	Metal(const Vec3 &_albedo) : Metal(_albedo, 0) {}
	Metal(const Vec3 &_albedo, Real _roughness) { albedo = _albedo; roughness = clamp(_roughness, 0, 1); }

	virtual Vec3 shade(const Vec3 &incoming, const HitRecord &hr, const Scene &scene, uint bounces) const;
};

Vec3 Metal::shade(const Vec3 &incoming, const HitRecord &hr, const Scene &scene, uint bounces) const
{
	Real epsilon = 0.0001;
	Vec3 normal = hr.hitable->evaluateNormal(hr.point, epsilon);
	Vec3 reflected = reflect(incoming, normal);
	if (roughness)
		reflected += roughness * sampleUnitSphere();

	Vec3 irradiance;
	if (dot(reflected, normal) > 0)
	{

		// Push the origin of the scattered point slightly out of the surface to avoid self-intersection
		Ray scattered(hr.point + 0.0001 * normal, reflected);
		bool hit = false;
		HitRecord rec;
		Real dist = 0.0;
		uint nSamples = 100;
		uint iteration = 0;
		for (; iteration < nSamples; iteration++)
		{
			Vec3 point = scattered.to(dist);
			rec.t = max();
			hit = scene.hit(point, 1e-7, rec);
			dist += rec.t;

			if (hit || dist < rec.t || dist > 10)
				break;
		}

		if (hit)
		{
			if (bounces < 100)
				irradiance += rec.hitable->getMaterial()->shade(reflected, rec, scene, bounces + 1);
		}
		else
		{
			irradiance += scene.background().sample(reflected);
		}
	}

	return irradiance * albedo;
}
