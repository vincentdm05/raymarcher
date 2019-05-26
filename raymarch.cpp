#include "Common.hpp"

#include "Camera.hpp"
#include "Debug.hpp"
#include "Lambertian.hpp"
#include "Quat.hpp"
#include "Raymarcher.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Vec3.hpp"

#include <assert.h>
#include <iostream>

void testQuat()
{
	// Constructors and comparison ops
	Quat a0;
	assert(a0.s == 1 && a0.v == Vec3());
	Quat a1(1, Vec3(2, 3, 4));
	assert(a1.s = 1 && a1.v == Vec3(2, 3, 4));
	assert(a0 != a1);
	Quat a2(a1);
	assert(a2 == a1);
	Quat a3(Quat(1, Vec3(2, 3, 4)));
	assert(a3 == Quat(1, Vec3(2, 3, 4)));

	// Assignment ops
	Quat b0 = a1;
	assert(b0 == a1);
	Quat b1 = Quat(1, Vec3(2, 3, 4));
	assert(b1 == Quat(1, Vec3(2, 3, 4)));

	// Sign ops
	Quat c0(1, Vec3(2, 3, 4));
	assert(+c0 == Quat(1, Vec3(2, 3, 4)));
	assert(-c0 == Quat(-1, -Vec3(2, 3, 4)));

	// Subscript ops
	Quat d0(1, Vec3(2, 3, 4));
	assert(d0.s == d0[0] && d0.v.x == d0[1] && d0.v.y == d0[2] && d0.v.z == d0[3]);
	try
	{
		d0[4];
		assert(0 && "Index out of bounds should have thrown an exception.");
	}
	catch (const char *msg)
	{}
	d0[0] = 5;
	assert(d0.s == 5);
	d0[1] = 6;
	assert(d0.v[0] == 6);

	// Arithmetic assignment ops
	Quat e0;
	e0 += Quat(1, Vec3());
	assert(e0 == Quat(2, Vec3()));
	e0 -= Quat(0, Vec3(1, 0, 0));
	assert(e0 == Quat(2, Vec3(-1, 0, 0)));
	// Quaternion representing a pi/2 rotation around the x axis
	Quat e1 = axisAngleToQuat(Vec3(1, 0, 0), M_PI * 0.5);
	e1 *= e1;
	Real angle;
	Vec3 axis;
	e1.getAxisAngle(axis, angle);
	raymAssert(axis.normalize() == Vec3(1, 0, 0), axis, " != ", Vec3(1, 0, 0));
	raymAssert(closeEnough(angle, M_PI, 0.0001), angle, " != ", M_PI);
	e1 /= e1;
	raymAssert(e1 == Quat(), e1, " != ", Quat());
	Quat e2(1, Vec3(-1, 0, 1));
	e2 += 1;
	raymAssert(e2 == Quat(2, Vec3(0, 1, 2)), e2);
	e2 -= 2;
	raymAssert(e2 == Quat(0, Vec3(-2, -1, 0)), e2);
	e2 *= -2;
	raymAssert(e2 == Quat(0, Vec3(4, 2, 0)), e2);
	e2 /= 2;
	raymAssert(e2 == Quat(0, Vec3(2, 1, 0)), e2);

	// Mathematical properties
	Quat f0(1, Vec3(2, 3, 4));
	raymAssert(f0.getConjugate() == Quat(1, -Vec3(2, 3, 4)), f0.getConjugate());
	raymAssert(f0.conjugate() == Quat(1, -Vec3(2, 3, 4)), f0);
	Quat f1(-2, Vec3(2, -2, 2));
	assert(f1.squaredNorm() == 16);
	assert(f1.norm() == 4);
	assert(f1.normalized().norm() == 1);
	assert(f1.normalize().norm() == 1);
	Quat f2(1, Vec3(-1, 1, -1));
	raymAssert(f2.reciprocal() == Quat(0.25, Vec3(0.25, -0.25, 0.25)), f2.reciprocal());
	raymAssert(f2.reciprocate() == Quat(0.25, Vec3(0.25, -0.25, 0.25)), f2);

	// Arithmetic ops
	assert(Quat(0, Vec3(1, 2, 3)) + Quat(1, Vec3(-1, 0, -2)) == Quat(1, Vec3(0, 2, 1)));
	assert(Quat(0, Vec3(1, 2, 3)) - Quat(1, Vec3(-1, 0, -2)) == Quat(-1, Vec3(2, 2, 5)));
	Quat g0 = axisAngleToQuat(Vec3(1, 0, 0), M_PI * 0.5);
	Quat g1 = axisAngleToQuat(Vec3(1, 0, 0), M_PI);
	Quat g2 = axisAngleToQuat(Vec3(1, 0, 0), M_PI * 1.5);
	raymAssert(closeEnough(g1 * g0, g2, 0.0001), g1 * g0, " != ", g2);
	raymAssert(closeEnough(g1 / g0, g0, 0.0001), g1 / g0, " != ", g0);
	assert(Quat(0, Vec3(1, 2, 3)) + 2 == Quat(2, Vec3(3, 4, 5)));
	assert(Quat(0, Vec3(1, 2, 3)) - 2 == Quat(-2, Vec3(-1, 0, 1)));
	assert(Quat(0, Vec3(-1, 2, 3)) * -1 == Quat(0, Vec3(1, -2, -3)));
	assert(Quat(0, Vec3(1, -2, 4)) / 2 == Quat(0, Vec3(0.5, -1, 2)));
	assert(2 + Quat(0, Vec3(1, -2, -1)) == Quat(2, Vec3(3, 0, 1)));
	assert(2 - Quat(0, Vec3(-1, 2, 3)) == Quat(2, Vec3(3, 0, -1)));
	assert(2 * Quat(0, Vec3(-1, 2, 3)) == Quat(0, Vec3(-2, 4, 6)));
	assert(closeEnough(1 / g0, axisAngleToQuat(Vec3(1, 0, 0), -M_PI * 0.5), 0.0001));

	// Free function mathematical properties
	assert(dot(Quat(1, Vec3(-1, 2, -2)), Quat(-2, Vec3(-1, 2, 1))) == 1);
	assert(conjugate(Quat(1, Vec3(2, -3, 4))) == Quat(1, Vec3(-2, 3, -4)));
	Quat h0(-1, Vec3(2, -3, 4));
	assert(normalize(h0) == h0.normalized());
	raymAssert(closeEnough(reciprocate(h0), h0.reciprocal(), 0.0001), reciprocate(h0), " != ", h0.reciprocal());
	assert(closeEnough(axisAngleToQuat(Vec3(1, 0, 0), M_PI * 0.5), Quat(0.7071067811882787, Vec3(0.7071067811848163, 0.0, 0.0)), 0.0001));
	Vec3 rotated = rotate(Vec3(1, 1, 0), axisAngleToQuat(Vec3(0, 0, 1), M_PI * 0.5));
	raymAssert(closeEnough(rotated, Vec3(-1, 1, 0), 0.0001), rotated);

	std::cout << "All tests passed!" << std::endl;
}

void testScene(const Raymarcher &raymarcher)
{
	Vec3 cameraPosition(0.0, 0.0, 1.0);
	Vec3 focusPosition(0, 0, -3);
	Vec3 focusDirection = focusPosition - cameraPosition;
	Camera camera(cameraPosition, focusDirection, Vec3(0.0, 1.0, 0.0), 20, Viewport(200, 100));

	Lambertian material(Vec3(0.1, 0.2, 0.5));

	Sphere sphere(focusPosition, 0.5, material);

	Scene scene;
	scene.setBackground(Background(Vec3(0.8, 0.3, 0.1), Vec3(0.2, 0.7, 0.9)));
	scene.add(sphere);

	raymarcher.printImage(scene, camera);
}

int main()
{
	// testQuat();

	Raymarcher raymarcher;
	raymarcher.setMaxRayIterations(30);
	raymarcher.setHitEpsilon(0.0001);
	raymarcher.setSamplesPerPixel(10);

	testScene(raymarcher);

	return 0;
}
