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

#include <atomic>
#include <future>

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
	void renderPixels(const Camera &camera, const Scene &scene, const Viewport &vp, std::atomic<uint> &counter, Framebuffer &framebuffer) const;
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

void Raymarcher::renderPixels(const Camera &camera, const Scene &scene, const Viewport &vp, std::atomic<uint> &counter, Framebuffer &framebuffer) const
{
	uint pixelAmount = vp.width() * vp.height();
	while (true)
	{
		uint pixelIndex = counter++;
		if (pixelIndex >= pixelAmount)
			break;

		uint row = pixelIndex / vp.width();
		uint col = pixelIndex % vp.width();
		Real u = Real(col);
		Real v = Real(row);
		if (samplesPerPixel > 1)
		{
			u += uniformRand();
			v += uniformRand();
		}
		else
		{
			u += 0.5;
			v += 0.5;
		}
		u /= vp.width();
		v /= vp.height();
		Ray r = camera.getRay(u, v);
		Vec3 colour = getColour(r, scene);

		for (uint i = 1; i < samplesPerPixel; i++)
		{
			u = Real(col + uniformRand()) / vp.width();
			v = Real(row + uniformRand()) / vp.height();
			r = camera.getRay(u, v);
			colour += getColour(r, scene);
		}

		colour /= samplesPerPixel;
		colour = 255.99 * gammaCorrect(colour);

		Real colourArray[3];
		colourArray[0] = colour.r;
		colourArray[1] = colour.g;
		colourArray[2] = colour.b;
		framebuffer.store(int(col), int(row), (byte*)colourArray);
	}
}

void Raymarcher::render(const Scene &scene, const Camera &camera, Framebuffer &framebuffer) const
{
	std::atomic<uint> renderPixelCounter {0};
	const Viewport &viewport = camera.getViewport();

	const uint nThreads = max(std::thread::hardware_concurrency(), 1) - 1;
	std::future<void> futures[nThreads];
	for (uint i = 0; i < nThreads; i++)
		futures[i] = std::async(std::launch::async, &Raymarcher::renderPixels, this, std::ref(camera), std::ref(scene), std::ref(viewport), std::ref(renderPixelCounter), std::ref(framebuffer));

	renderPixels(camera, scene, viewport, renderPixelCounter, framebuffer);

	for (uint i = 0; i < nThreads; i++)
		futures[i].get();
}
