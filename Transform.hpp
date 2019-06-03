#pragma once

#include "Common.hpp"

#include "Quat.hpp"
#include "Vec3.hpp"

class Transform
{
private:
	Quat r;
	Vec3 t;
	Real s = 1;

public:
	Transform() {}
	Transform(const Quat &_rotation, const Vec3 &_translation, Real _scale) { r = _rotation; t = _translation; s = _scale; }

	inline bool operator==(const Transform &_t) const { return r == _t.r && t == _t.t && s == _t.s; }
	inline bool operator!=(const Transform &_t) const { return r != _t.r || t != _t.t || s != _t.s; }

	const Quat &rotation() const { return r; }
	const Vec3 &translation() const { return t; }
	Real scale() const { return s; }
	void setRotation(const Quat &_r) { r = _r; }
	void setTranslation(const Vec3 &_t) { t = _t; }
	void setScale(Real _s) { s = _s; }

	Transform inverse() const;
	Vec3 apply(const Vec3 &v) const;
	Vec3 applyInverse(const Vec3 &v) const;
};

Transform Transform::inverse() const
{
	Quat inverseRotation = r.getConjugate();
	Real inverseScale = 1.0 / s;
	return Transform(inverseRotation, -rotate(t, inverseRotation) * inverseScale, inverseScale);
}

Vec3 Transform::apply(const Vec3 &v) const
{
	Vec3 u = v * s;
	u = rotate(u, r);
	u += t;
	return u;
}

Vec3 Transform::applyInverse(const Vec3 &v) const
{
	Vec3 u = v - t;
	u = rotate(u, r.getConjugate());
	u /= s;
	return u;
}

inline std::istream &operator>>(std::istream &is, Transform &t)
{
	Quat r;
	is >> r.s >> r.v;
	Vec3 tr;
	is >> tr;
	Real s;
	is >> s;
	t.setRotation(r);
	t.setTranslation(tr);
	t.setScale(s);
	return is;
}

inline std::ostream &operator<<(std::ostream &os, const Transform &t)
{
	os << "Transform(" << t.rotation() << ", " << t.translation() << ", " << t.scale() << ")";
	return os;
}

inline bool closeEnough(const Transform &a, const Transform &b, Real epsilon)
{
	return closeEnough(a.scale(), b.scale(), epsilon) &&
		closeEnough(a.rotation(), b.rotation(), epsilon) &&
		closeEnough(a.translation(), b.translation(), epsilon);
}
