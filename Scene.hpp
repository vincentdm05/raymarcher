#pragma once

#include "Common.hpp"

#include "Background.hpp"
#include "Hitable.hpp"
#include "Math.hpp"

#include <vector>

class Scene : public Hitable
{
private:
	Background bg;
	std::vector<const Hitable *> hitables;

public:
	Scene() {}
	Scene(uint size) { hitables.reserve(size); }
	~Scene() { hitables.clear(); }

	virtual bool hit(const Vec3 &point, Real epsilon, HitRecord &rec) const override;
	virtual Real evaluateSDF(const Vec3 &point) const override;

	void setBackground(const Background &_background) { bg = _background; }
	const Background &background() const { return bg; }
	void add(const Hitable &hitable) { hitables.push_back(&hitable); }
};

bool Scene::hit(const Vec3 &point, Real epsilon, HitRecord &rec) const
{
	for (const Hitable *hitable : hitables)
	{
		HitRecord tmpRec;
		if (hitable->hit(point, epsilon, tmpRec))
		{
			rec = tmpRec;
			return true;
		}
		else if (tmpRec.t < rec.t)
		{
			rec = tmpRec;
		}
	}
	return false;
}

Real Scene::evaluateSDF(const Vec3 &point) const
{
	Real minDist = max();
	for (const Hitable *hitable : hitables)
		minDist = min(minDist, hitable->evaluateSDF(point));
	return minDist;
}
