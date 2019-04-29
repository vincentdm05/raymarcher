#pragma once

#include "Common.hpp"

#include "Background.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "Math.hpp"
#include "Ray.hpp"
#include "Scene.hpp"
#include "Vec3.hpp"
#include "Viewport.hpp"

#include <iostream>
#include <limits>
#include <random>

class Raymarcher
{
private:
	bool outputEnabled = true;
	uint maxRayIterations = 100;
	uint samplesPerPixel = 100;
	Real hitEpsilon = 1e-4;

	Vec3 gammaCorrect(const Vec3 &colour) const;

public:
	Raymarcher() {}

	Vec3 getColour(const Ray &r, const Scene &scene) const;
	Vec3 samplePixel(const Camera &camera, const Scene &scene, int col, int row, const Viewport &vp, uint nSamples = 1) const;
	void printImage(const Scene &scene, const Camera &camera) const;
	void setOutputEnabled(bool value) { outputEnabled = value; }
	void setMaxRayIterations(uint n) { maxRayIterations = n; }
	void setSamplesPerPixel(uint n) { samplesPerPixel = n; }
	void setHitEpsilon(Real e) { hitEpsilon = e; }
};

Vec3 Raymarcher::gammaCorrect(const Vec3 &colour) const
{
	// Gamma 2 correction
	return sqrt(colour);
}

Vec3 Raymarcher::getColour(const Ray &r, const Scene &scene) const
{
	bool hit = false;
	HitRecord rec;
	Real dist = 0.0;
	for (uint i = 0; i < maxRayIterations; i++)
	{
		Vec3 point = r.origin() + r.direction() * dist;
		rec.t = std::numeric_limits<Real>::max();
		hit = scene.evaluateSDF(point, hitEpsilon, rec);
		dist += rec.t;

		if (hit || dist < rec.t)
			break;
	}

	if (hit)
	{
		if (rec.material)
			return rec.material->shade(rec, scene);
		else
			return Vec3(0, 0, 0);
	}

	return scene.background().sample(r.direction());
}

Vec3 Raymarcher::samplePixel(const Camera &camera, const Scene &scene, int col, int row, const Viewport &vp, uint nSamples) const
{
	Vec3 colour;
	for (uint i = 0; i < nSamples; i++)
	{
		Real uOffset = 0.5;
		Real vOffset = 0.5;
		if (nSamples > 1)
		{
			uOffset = drand48();
			vOffset = drand48();
		}
		Real u = Real(col + uOffset) / vp.width();
		Real v = Real(row + vOffset) / vp.height();
		Ray r = camera.getRay(u, v);
		colour += getColour(r, scene);
	}
	return colour / nSamples;
}

void Raymarcher::printImage(const Scene &scene, const Camera &camera) const
{
	const Viewport &viewport = camera.getViewport();

	int nx = viewport.width();
	int ny = viewport.height();
	if (outputEnabled)
		std::cout << "P3\n" << nx << " " << ny << "\n255\n";

	for (int row = ny - 1; row >= 0; row--)
	{
		for (int col = 0; col < nx; col++)
		{
			Vec3 colour = samplePixel(camera, scene, col, row, viewport, samplesPerPixel);
			colour = 255.99 * gammaCorrect(colour);

			if (outputEnabled)
				std::cout << int(colour.r) << " " << int(colour.g) << " " << int(colour.b) << "\n";
		}
	}
}
