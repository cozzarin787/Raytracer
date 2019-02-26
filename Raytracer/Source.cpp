#include "Camera.h"
#include <iostream>
#include "lodepng.h"
#include "Sphere.h"
#include "Polygon.h"
#include "Triangle.h"
#include "Phong.h"
#define print(x) std::cout << x << std::endl;

// Glass Sphere
Color colorGlass = Color(1, 1, 0);
Color specColorGlass = Color(1, 1, 1);
Phong matGlass = Phong(colorGlass, specColorGlass, 0.0f, 0.75f, 0.25f, 100.0f);
Point centerGlass = Point(-0.121f, 1.014f, -4.724f);
float radiusGlass = 1.2988f;

// Mirror Sphere
Color colorMirror = Color(0, 0, 1);
Color specColorMirror = Color(1, 1, 1);
Phong matMirror = Phong(colorMirror, specColorMirror, 0.0f, 0.70f, 0.30f, 10.0f);
Point centerMirror = Point(1.5f, 0.2f, -3.94f);
float radiusMirror = 1.11f;

//Floor
Color colorFloor = Color(1, 0, 0);
Color specColorFloor = Color(1, 1, 1);
Phong matFloor = Phong(colorFloor, specColorFloor, 0.0f, 1.0f, 0.0f, 1.0f);
std::vector<Point> floorVertices{Point(-0.85f, -0.5f, 1), Point(-1, -0.5f, 10), Point(2, -0.5f, 10), Point(2, -0.5f, 1)};

//Triangles
Color colorTri = Color(1, 0, 0);
Phong matTri = Phong(colorTri, specColorFloor, 0.0f, 1.0f, 0.0f, 1.0f);
Point p0 = Point(-2.7f, -1, -10);
Point p1 = Point(-2.7f, -1, 20);
Point p2 = Point(9, -1, 20);

Color colorTri2 = Color(1, 0, 0);
Phong matTri2 = Phong(colorTri2, specColorFloor, 0.0f, 1.0f, 0.0f, 1.0f);
Point p3 = Point(9, -1, 20);
Point p4 = Point(9, -1, -10);
Point p5 = Point(-2.7f, -1, -10);

int main()
{
	//Scaling Matrix
	Matrix<float, 4, 4, RowMajor> scalingMatrix;
	scalingMatrix.row(0) << .57955f, 0, 0, 0;
	scalingMatrix.row(1) << 0, 1, 0, 0;
	scalingMatrix.row(2) << 0, 0, 1.79781f, 0;
	scalingMatrix.row(3) << 0, 0, 0, 1;

	// Create Objects
	Material* m1 = &matGlass;
	Material* m2 = &matMirror;
	Material* m3 = &matFloor;
	Material* m4 = &matTri;
	Material* m5 = &matTri2;

	Sphere sphereGlass = Sphere(m1, centerGlass, radiusGlass);
	Sphere sphereMirror = Sphere(m2, centerMirror, radiusMirror);
	Polygon floor = Polygon(m3, floorVertices);
	Triangle t1 = Triangle(m4, p0, p1, p2);
	Triangle t2 = Triangle(m5, p3, p4, p5);

	// Wrap objects in object pointers to induce polymorphism
	Object* o1 = &sphereGlass;
	Object* o2 = &sphereMirror;
	Object* o3 = &floor;
	Object* o4 = &t1;
	Object* o5 = &t2;

	// Create LightSources
	Point lightPoint1 = Point(-0.121f, 7.014f, -6.0f);
	LightSource l1 = LightSource(lightPoint1, Color(1, 1, 1));

	// Add objects to world
	World world = World(Color(0.11765f, 0.56471f, 1));

	int glassIndex = world.add(o1);
	int mirrorIndex = world.add(o2);
	//int floorIndex = world.add(o3);
	int triIndex1 = world.add(o4);
	int triIndex2 = world.add(o5);

	// Add lights to world
	int light1Index = world.addLight(l1);

	// translate, rotate, and scale world objects to taste
	//world.transform(floorIndex, scalingMatrix);

	// Define camera
	Camera c = Camera(Point(0, 0.941f, -10), RowVector3f(0, 0, 1), RowVector3f(0, 1, 0));
	c.setFocalLength(1);
	c.setFilmPlaneDim(60, (4 / 3.0f));
	c.setImageDim(512, 384);

	c.render(world);

	print(world.toString());
	print(c.toString());

	std::cin.ignore();
	return 0;
}