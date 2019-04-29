#pragma once

#include "Common.hpp"

#include "Background.hpp"
#include "Hitable.hpp"

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

	virtual bool evaluateSDF(const Vec3 &point, Real epsilon, HitRecord &rec) const;

	void setBackground(const Background &_background) { bg = _background; }
	const Background &background() const { return bg; }
	void add(const Hitable &hitable) { hitables.push_back(&hitable); }
};

bool Scene::evaluateSDF(const Vec3 &point, Real epsilon, HitRecord &rec) const
{
	for (const Hitable *hitable : hitables)
	{
		HitRecord tmpRec;
		if (hitable->evaluateSDF(point, epsilon, tmpRec))
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
