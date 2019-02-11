#include "Camera.h"
#include <iostream>
#include "lodepng.h"
#include "Sphere.h"
#include "Polygon.h"
#define print(x) std::cout << x << std::endl;

// Glass Sphere
Color colorGlass = Color(191, 63, 63, 255);
Material matGlass = Material(colorGlass);
Point centerGlass = Point(-0.121, 1.014, -7.724);
float radiusGlass = 0.016988;

// Mirror Sphere
Color colorMirror = Color(19, 15, 255, 255);
Material matMirror = Material(colorMirror);
Point centerMirror = Point(0.86, 0.35, -6.94);
float radiusMirror = 0.012567;

//Floor
Color colorFloor = Color(44, 220, 47, 255);
Material matFloor = Material(colorFloor);
std::vector<Point> floorVertices{Point(-1.4 ,0 ,-9), Point(-1.4, 0, 9), Point(1.4, 0 , 9), Point(1.4, 0, -9)};

std::vector<Point> floorVertices1{ Point(100 , 100, -7), Point(-100, 100, -7), Point(-100, -100 , -7), Point(100, -100, -7) };

int main()
{
	//Scaling Matrix
	Matrix4f scalingMatrix;
	scalingMatrix.row(0) << .57955, 0, 0, 0;
	scalingMatrix.row(1) << 0, 1, 0, 0;
	scalingMatrix.row(2) << 0, 0, 1.79781, 0;
	scalingMatrix.row(3) << 0, 0, 0, 1;

	// Create Objects
	Sphere sphereGlass = Sphere(matGlass, centerGlass, radiusGlass);
	Sphere sphereMirror = Sphere(matMirror, centerMirror, radiusMirror);
	Polygon floor = Polygon(matFloor, floorVertices1);

	// Wrap objects in object pointers to induce polymorphism
	Object* o1 = &sphereGlass;
	Object* o2 = &sphereMirror;
	Object* o3 = &floor;

	// Add objects to world
	World world = World();

	int glassIndex = world.add(o1);
	int mirrorIndex = world.add(o2);
	int floorIndex = world.add(o3);

	// translate, rotate, and scale world objects to taste
	//world.transform(floorIndex, scalingMatrix);

	// Define camera
	Camera c = Camera(Point(0, 0.941, -10), RowVector3f(0, 0, 1), RowVector3f(0, 1, 0));
	c.setFocalLength(10);
	c.setFilmPlaneDim(100, (4 / 3.0f));
	c.setImageDim(512, 368);

	c.render(world);
	//world.transformAllObjects(c.viewTransform);
	print(world.toString());
	print(c.toString());

	std::cin.ignore();
	return 0;
}