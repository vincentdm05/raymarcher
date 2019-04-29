#include "Common.hpp"

#include "Camera.hpp"
#include "Lambertian.hpp"
#include "Raymarcher.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Vec3.hpp"

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
	Raymarcher raymarcher;
	raymarcher.setMaxRayIterations(30);
	raymarcher.setHitEpsilon(0.0001);
	raymarcher.setSamplesPerPixel(10);

	testScene(raymarcher);

	return 0;
}
