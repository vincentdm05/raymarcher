#pragma once

#include "Common.hpp"

#include "Background.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "Math.hpp"
#include "Ray.hpp"
#include "Sampling.hpp"
#include "Scene.hpp"
#include "Vec3.hpp"
#include "Viewport.hpp"

#include <iostream>
#include <random>

class Raymarcher
{
private:
	bool visualiseIterations = false;
	bool visualiseDepth = false;
	uint maxRayIterations = 300;
	uint samplesPerPixel = 1;
	Real maxRayLength = 1000.0;
	Real hitEpsilon = 1e-4;

	Vec3 gammaCorrect(const Vec3 &colour) const;

public:
	Raymarcher() {}

	Vec3 getColour(const Ray &r, const Scene &scene) const;
	Vec3 renderPixel(const Camera &camera, const Scene &scene, int col, int row, const Viewport &vp, uint nSamples = 1) const;
	void render(const Scene &scene, const Camera &camera, Framebuffer &framebuffer) const;
	void setVisualiseIterations(bool value) { visualiseIterations = value; }
	void setVisualiseDepth(bool value) { visualiseDepth = value; }
	void setMaxRayIterations(uint n) { maxRayIterations = n; }
	void setSamplesPerPixel(uint n) { samplesPerPixel = n; }
	void setMaxRayLength(Real l) { maxRayLength = l; }
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
	uint iteration = 0;
	for (; iteration < maxRayIterations; iteration++)
	{
		Vec3 point = r.to(dist);
		rec.t = max();
		hit = scene.hit(point, hitEpsilon, rec);
		dist += rec.t;

		if (hit || dist < rec.t || dist > maxRayLength)
			break;
	}

	if (visualiseIterations)
		return Vec3(1, 1, 1) * iteration / maxRayIterations;
	else if (visualiseDepth)
		return Vec3(1, 1, 1) / (1.0 + dist);

	if (hit)
	{
		if (rec.hitable)
		{
			return rec.hitable->getMaterial()->shade(rec, scene);
		}
		else
		{
			return Vec3(0, 0, 0);
		}
	}

	return scene.background().sample(r.direction());
}

Vec3 Raymarcher::renderPixel(const Camera &camera, const Scene &scene, int col, int row, const Viewport &vp, uint nSamples) const
{
	Vec3 colour;
	for (uint i = 0; i < nSamples; i++)
	{
		Real uOffset = 0.5;
		Real vOffset = 0.5;
		if (nSamples > 1)
		{
			uOffset = uniformRand();
			vOffset = uniformRand();
		}
		Real u = Real(col + uOffset) / vp.width();
		Real v = Real(row + vOffset) / vp.height();
		Ray r = camera.getRay(u, v);
		colour += getColour(r, scene);
	}
	return colour / nSamples;
}

void Raymarcher::render(const Scene &scene, const Camera &camera, Framebuffer &framebuffer) const
{
	const Viewport &viewport = camera.getViewport();

	int nx = viewport.width();
	int ny = viewport.height();

	Real colourArray[3];
	for (int row = ny - 1; row >= 0; row--)
	{
		for (int col = 0; col < nx; col++)
		{
			Vec3 colour = renderPixel(camera, scene, col, row, viewport, samplesPerPixel);
			colour = 255.99 * gammaCorrect(colour);

			colourArray[0] = colour.r;
			colourArray[1] = colour.g;
			colourArray[2] = colour.b;
			framebuffer.store(col, row, (byte*)colourArray);
		}
	}
}
