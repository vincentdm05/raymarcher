#pragma once

#include "Common.hpp"

#include "Vec3.hpp"

#include <math.h>

class Quat
{
private:
	Vec3 vector;
	Real scalar;

public:
	Real &w = scalar;
	Real &x = vector[0];
	Real &y = vector[1];
	Real &z = vector[2];
	Real &s = scalar;
	Vec3 &v = vector;

	Quat() { w = 0; x = 1; y = 0; z = 0; }
	Quat(Real _w, Real _x, Real _y, Real _z) { w = _w; x = _x; y = _y; z = _z; }
	Quat(Real _s, const Vec3 &_v) { s = _s; v = _v; }

	inline Quat &operator+() { return *this; }
	inline Quat operator-() const { return Quat(-s, -v); }
	inline Real operator[](uint i) const { if (i == 0) return s; return v[i - 1]; }
	inline Real &operator[](uint i) { if (i == 0) return s; return v[i - 1]; }

	inline Quat &operator+=(const Quat &q);
	inline Quat &operator-=(const Quat &q);
	inline Quat &operator*=(const Quat &q);
	inline Quat &operator/=(const Quat &q);
	inline Quat &operator+=(Real c);
	inline Quat &operator-=(Real c);
	inline Quat &operator*=(Real c);
	inline Quat &operator/=(Real c);

	inline Quat getConjugate() const { return Quat(s, -v); }
	inline Quat &conjugate();
	inline Real squaredNorm() const { return s * s + v.squaredLength(); }
	inline Real norm() const { return sqrt(squaredNorm()); }
	inline Quat &normalize();
	inline Quat reciprocal() const;
	inline Quat &reciprocate();
};

inline Quat &Quat::operator+=(const Quat &q)
{
	s += q.s;
	v += q.v;
	return *this;
}

inline Quat &Quat::operator-=(const Quat &q)
{
	s -= q.s;
	v -= q.v;
	return *this;
}

inline Quat &Quat::operator*=(const Quat &q)
{
	s *= q.s;
	v *= q.v;
	return *this;
}

inline Quat &Quat::operator/=(const Quat &q)
{
	s /= q.s;
	v /= q.v;
	return *this;
}

inline Quat &Quat::operator+=(Real c)
{
	s += c;
	v += c;
	return *this;
}

inline Quat &Quat::operator-=(Real c)
{
	s -= c;
	v -= c;
	return *this;
}

inline Quat &Quat::operator*=(Real c)
{
	s *= c;
	v *= c;
	return *this;
}

inline Quat &Quat::operator/=(Real c)
{
	s /= c;
	v /= c;
	return *this;
}

inline Quat &Quat::conjugate()
{
	v = -v;
	return *this;
}

inline Quat &Quat::normalize()
{
	Real invn = 1.0 / norm();
	s *= invn;
	v *= invn;
	return *this;
}

inline Quat Quat::reciprocal() const
{
	Quat q = getConjugate();
	q /= squaredNorm();
	return q;
}

inline Quat &Quat::reciprocate()
{
	Real sqnorm = squaredNorm();
	conjugate();
	*this /= sqnorm;
	return *this;
}

inline std::istream &operator>>(std::istream &is, Quat &q)
{
	is >> q.s >> q.v;
	return is;
}

inline std::ostream &operator<<(std::ostream &os, const Quat &q)
{
	os << q.s << " " << q.v;
	return os;
}

inline Quat operator+(const Quat &a, const Quat &b)
{
	return Quat(a.s + b.s, a.v + b.v);
}

inline Quat operator-(const Quat &a, const Quat &b)
{
	return Quat(a.s - b.s, a.v - b.v);
}

inline Quat operator*(const Quat &a, const Quat &b)
{
	return Quat(a.s * b.s - dot(a.v, b.v), a.s * b.v + b.s * a.v + cross(a.v, b.v));
}

inline Quat operator/(const Quat &a, const Quat &b)
{
	return a * b.reciprocal();
}

inline Quat operator+(const Quat &q, Real c)
{
	return Quat(q.s + c, q.v + c);
}

inline Quat operator-(const Quat &q, Real c)
{
	return Quat(q.s - c, q.v - c);
}

inline Quat operator*(const Quat &q, Real c)
{
	return Quat(q.s * c, q.v * c);
}

inline Quat operator/(const Quat &q, Real c)
{
	return Quat(q.s / c, q.v / c);
}

inline Quat operator+(Real c, const Quat &q)
{
	return q + c;
}

inline Quat operator-(Real c, const Quat &q)
{
	return -q + c;
}

inline Quat operator*(Real c, const Quat &q)
{
	return q * c;
}

inline Quat operator/(Real c, const Quat &q)
{
	return q.reciprocal() * c;
}

inline Real dot(const Quat &a, const Quat &b)
{
	return a.s * b.s + dot(a.v, b.v);
}

inline Quat conjugate(const Quat &q)
{
	return Quat(q.s, -q.v);
}

inline Quat normalize(const Quat &q)
{
	Real invn = 1.0 / q.norm();
	return q * invn;
}

inline Quat reciprocate(const Quat &q)
{
	return q.getConjugate() / q.squaredNorm();
}

inline Vec3 rotate(const Vec3 &v, const Quat &q)
{
	Vec3 res = dot(v, q.v) * q.v;
	res += q.s * cross(q.v, v);
	res *= 2;
	res += (q.s * q.s - q.v.squaredLength()) * v;
	return res;
}
