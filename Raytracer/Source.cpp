#include "Camera.h"
#include <iostream>
#include "lodepng.h"
#include "Sphere.h"
#include "Polygon.h"
#include "Triangle.h"
#include "Phong.h"
#include "Checkerboard.h"

#include <string>
#include <sstream>
#include <fstream>
#include <math.h>

#define print(x) std::cout << x << std::endl;

std::vector<Triangle> makeStanfordBunny(std::string fileName)
{
	std::vector<Point> vertices;
	std::vector<Triangle> bunny;
	int numVertices;
	int numTriangles;
	int i = 1;

	std::ifstream inFile(fileName);
	std::string line;
	
	// skip first line
	std::getline(inFile, line);
	
	while (std::getline(inFile, line))
	{
		std::istringstream iss(line);
		if (i == 1)
		{
			std::string hashtag, vertex, count, eq;
			iss >> hashtag >> vertex >> count >> eq >> numVertices;
		}
		else if (i == 2)
		{
			std::string hashtag, face, count, eq;
			iss >> hashtag >> face >> count >> eq >> numTriangles;
		}
		else if (i < numVertices + 3)
		{
			std::string v;
			float x, y, z;
			iss >> v >> x >> y >> z;
			vertices.push_back(Point(x, y, z));
		}
		else
		{
			std::string f;
			int p0, p1, p2;
			iss >> f >> p0 >> p1 >> p2;

			Color c = Color(0.7, 0.7, 0.7);
			Color cSpec = Color(1, 1, 1);
			Material* mt = new Phong(c, cSpec, 0.0f, 0.75f, 0.25f, 100.0f);

			bunny.push_back(Triangle(mt, vertices[p0-1], vertices[p1-1], vertices[p2-1]));
		}
		i++;
	}
	
	return bunny;
}

void createWorld()
{
	// Glass Sphere
	Color colorGlass = Color(0, 1, 0);
	Color specColorGlass = Color(1, 1, 1);
	Phong matGlass = Phong(colorGlass, specColorGlass, 0.0f, 0.75f, 0.25f, 100.0f);
	Point centerGlass = Point(0.0f, 0.0f, 0.0f);
	Point centerGlass2 = Point(0.0f, 2.5f, 0.0f);
	Point centerGlass3 = Point(-3.0f, 0.0f, 0.0f);
	float radiusGlass = 1.0f;

	//Scaling Matrix
	Matrix4f scalingMatrix;
	scalingMatrix.row(0) << 1, 0, 0, 0.0;
	scalingMatrix.row(1) << 0, 1, 0, 0.0;
	scalingMatrix.row(2) << 0, 0, 1, 0.0;
	scalingMatrix.row(3) << 0, 0, 0, 1;

	// Create Objects
	Material* m1 = &matGlass;

	Sphere sphereGlass = Sphere(m1, centerGlass, radiusGlass);
	Sphere s2 = Sphere(m1, centerGlass2, radiusGlass);
	Sphere s3 = Sphere(m1, centerGlass3, radiusGlass);

	// Wrap objects in object pointers to induce polymorphism
	Object* o1 = &sphereGlass;
	Object* o2 = &s2;
	Object* o3 = &s3;

	// Create LightSources
	Point lightPoint1 = Point(-6.0f, 3.0f, 0.0f);
	LightSource l1 = LightSource(lightPoint1, Color(1, 1, 1));

	// Add objects to world
	World world = World(Color(0.11765f, 0.56471f, 1));

	int glassIndex = world.add(o1);
	world.add(o2);
	//world.add(o3);

	// Translate object
	//world.transform(glassIndex, scalingMatrix);

	// Add lights to world
	int light1Index = world.addLight(&l1);

	// Define camera 
	Camera c = Camera(Point(0.0f, 2.5f, -9.0f), Vector3f(0, 2.5f, 0), Vector3f(0, 1, 0));
	c.setFocalLength(1);
	c.setFilmPlaneDim(60, (4 / 3.0f));
	c.setImageDim(512, 384);
	c.setSpatialDataStructure(1);

	c.render(world);

	print(world.toString());
	print(c.toString());

	std::cin.ignore();
}

void createScene1()
{
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
	
	//Checker Board Triangles
	Color color1 = Color(1, 1, 0);
	Color color2 = Color(1, 0, 0);
	Color specColorFloor = Color(1, 1, 1);
	Checkerboard matTri = Checkerboard(color1, color2, Point(0, 0, 0), Point(0, 0, 1), Point(1, 0, 1), specColorFloor, 0.0f, 1.0f, 0.0f, 1.0f);
	Point p0 = Point(-2.7f, -1, -10);
	Point p1 = Point(-2.7f, -1, 20);
	Point p2 = Point(9, -1, 20);

	Checkerboard matTri2 = Checkerboard(color1, color2, Point(1, 0, 1), Point(1, 0, 0), Point(0, 0, 0), specColorFloor, 0.0f, 1.0f, 0.0f, 1.0f);
	Point p3 = Point(9, -1, 20);
	Point p4 = Point(9, -1, -10);
	Point p5 = Point(-2.7f, -1, -10);

	//Scaling Matrix
	Matrix4f scalingMatrix;
	scalingMatrix.row(0) << .57955f, 0, 0, 0;
	scalingMatrix.row(1) << 0, 1, 0, 0;
	scalingMatrix.row(2) << 0, 0, 1.79781f, 0;
	scalingMatrix.row(3) << 0, 0, 0, 1;

	// Create Objects
	Material* m1 = &matGlass;
	Material* m2 = &matMirror;
	Material* m4 = &matTri;
	Material* m5 = &matTri2;

	Sphere sphereGlass = Sphere(m1, centerGlass, radiusGlass);
	Sphere sphereMirror = Sphere(m2, centerMirror, radiusMirror);
	Triangle t1 = Triangle(m4, p0, p1, p2);
	Triangle t2 = Triangle(m5, p3, p4, p5);

	// Wrap objects in object pointers to induce polymorphism
	Object* o1 = &sphereGlass;
	Object* o2 = &sphereMirror;
	Object* o4 = &t1;
	Object* o5 = &t2;

	// Create LightSources
	Point lightPoint1 = Point(-2.0f, 3.014f, -10.0f);
	Point lightPoint2 = Point(2.0f, 1.0f, -10.0f);
	LightSource l1 = LightSource(lightPoint1, Color(1, 1, 1));
	LightSource l2 = LightSource(lightPoint2, Color(1, 1, 1));

	// Add objects to world
	World world = World(Color(0.11765f, 0.56471f, 1));

	int glassIndex = world.add(o1);
	int mirrorIndex = world.add(o2);
	//int floorIndex = world.add(o3);
	int triIndex1 = world.add(o4);
	int triIndex2 = world.add(o5);

	// Add lights to world
	int light1Index = world.addLight(&l1);
	//int light2Index = world.addLight(&l2);

	// Define camera 
	Camera c = Camera(Point(0, 0.941f, -10), Vector3f(0, 0, 1), Vector3f(0, 1, 0));
	c.setFocalLength(1);
	c.setFilmPlaneDim(60, (4 / 3.0f));
	c.setImageDim(512, 384);
	c.setSpatialDataStructure(1);

	c.render(world);

	print(world.toString());
	print(c.toString());

	std::cin.ignore();
}

void createBunnyScene()
{
	std::vector<Triangle> bunny = makeStanfordBunny("bunny.obj");

	// Rotation Matrix
	Matrix4f rotationMatrix;
	rotationMatrix.row(0) << cosf(3.14159f), 0, sinf(3.14159f), 0;
	rotationMatrix.row(1) << 0, 1, 0, 0;
	rotationMatrix.row(2) << -sinf(3.14159f), 0, cosf(3.14159f), 0;
	rotationMatrix.row(3) << 0, 0, 0, 1;

	// Translation Matrix
	Matrix4f translationMatrix;
	translationMatrix.row(0) << 1, 0, 0, -2;
	translationMatrix.row(1) << 0, 1, 0, -9.75;
	translationMatrix.row(2) << 0, 0, 1, 0;
	translationMatrix.row(3) << 0, 0, 0, 1;
	
	//Scaling Matrix
	Matrix4f scalingMatrix;
	scalingMatrix.row(0) << 100.0f, 0, 0, 0;
	scalingMatrix.row(1) << 0, 100.0f, 0, 0;
	scalingMatrix.row(2) << 0, 0, 100.0f, 0;
	scalingMatrix.row(3) << 0, 0, 0, 1;

	Matrix4f transform = translationMatrix * rotationMatrix * scalingMatrix;
	print(transform);

	// Create LightSources
	Point lightPoint1 = Point(-1.0f, 6.014f, -40.0f);
	LightSource l1 = LightSource(lightPoint1, Color(100, 100, 100));

	// Add objects to world
	World world = World(Color(0.11765f, 0.56471f, 1));

	// Add lights to world
	int light1Index = world.addLight(&l1);

	// Add bunny
	for (int i = 0; i < bunny.size(); i++)
	{
		Object * o = &bunny[i];
		int oIndex = world.add(o);
		world.transform(oIndex, transform);
	}

	// Define camera 
	Camera c = Camera(Point(0, 0.1f, -20), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
	c.setFocalLength(1);
	c.setFilmPlaneDim(60, (4 / 3.0f));
	c.setImageDim(512, 384);
	c.setSpatialDataStructure(1);

	c.render(world);

	//print(world.toString());
	//print(c.toString());

	std::cin.ignore();
}

int main(void)
{
	//createBunnyScene();
	createScene1();
	//createWorld();
	return 0;
}