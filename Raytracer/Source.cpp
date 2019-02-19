#include "Camera.h"
#include <iostream>
#include "lodepng.h"
#include "Sphere.h"
#include "Polygon.h"
#include "Triangle.h"
#define print(x) std::cout << x << std::endl;

// Glass Sphere
Color colorGlass = Color(191, 63, 63, 255);
Material matGlass = Material(colorGlass);
Point centerGlass = Point(-0.121, 1.014, -4.724);
float radiusGlass = 1.2988;

// Mirror Sphere
Color colorMirror = Color(19, 15, 255, 255);
Material matMirror = Material(colorMirror);
Point centerMirror = Point(1.5, 0.2, -3.94);
float radiusMirror = 1.11;

//Floor
Color colorFloor = Color(44, 220, 47, 255);
Material matFloor = Material(colorFloor);
std::vector<Point> floorVertices{Point(-0.85, -0.5, 1), Point(-1, -0.5, 10), Point(2, -0.5, 10), Point(2, -0.5, 1)};

//Triangles
Color colorTri = Color(255, 0, 0, 255);
Material matTri = Material(colorTri);
Point p0 = Point(-2.7, -1, -10);
Point p1 = Point(-2.7, -1, 20);
Point p2 = Point(9, -1, 20);

Color colorTri2 = Color(255, 0, 0, 255);
Material matTri2 = Material(colorTri2);
Point p3 = Point(9, -1, 20);
Point p4 = Point(9, -1, -10);
Point p5 = Point(-2.7, -1, -10);

Color colorTri3 = Color(255, 0, 0, 255);
Material matTri3 = Material(colorTri);
Point p6 = Point(0, 1, 20);
Point p7 = Point(-1, 0, 20);
Point p8 = Point(1, 0, 20);

int main()
{
	//Scaling Matrix
	Matrix<float, 4, 4, RowMajor> scalingMatrix;
	scalingMatrix.row(0) << .57955, 0, 0, 0;
	scalingMatrix.row(1) << 0, 1, 0, 0;
	scalingMatrix.row(2) << 0, 0, 1.79781, 0;
	scalingMatrix.row(3) << 0, 0, 0, 1;

	// Create Objects
	Sphere sphereGlass = Sphere(matGlass, centerGlass, radiusGlass);
	Sphere sphereMirror = Sphere(matMirror, centerMirror, radiusMirror);
	Polygon floor = Polygon(matFloor, floorVertices);
	Triangle t1 = Triangle(matTri, p0, p1, p2);
	Triangle t2 = Triangle(matTri2, p3, p4, p5);
	Triangle t3 = Triangle(matTri3, p6, p7, p8);

	// Wrap objects in object pointers to induce polymorphism
	Object* o1 = &sphereGlass;
	Object* o2 = &sphereMirror;
	Object* o3 = &floor;
	Object* o4 = &t1;
	Object* o5 = &t2;
	Object* o6 = &t3;

	// Add objects to world
	World world = World();

	int glassIndex = world.add(o1);
	int mirrorIndex = world.add(o2);
	//int floorIndex = world.add(o3);
	int triIndex1 = world.add(o4);
	int triIndex2 = world.add(o5);
	//int triIndex3 = world.add(o6);

	// translate, rotate, and scale world objects to taste
	//world.transform(floorIndex, scalingMatrix);

	// Define camera
	Camera c = Camera(Point(0, 0.941, -10), RowVector3f(0, 0, 1), RowVector3f(0, 1, 0));
	c.setFocalLength(1);
	c.setFilmPlaneDim(60, (4 / 3.0f));
	c.setImageDim(512, 384);

	c.render(world);

	print(world.toString());
	print(c.toString());

	std::cin.ignore();
	return 0;
}