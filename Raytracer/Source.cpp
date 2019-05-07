#include <iostream>
#include "Camera.h"
#include "lodepng.h"
#include "Sphere.h"
#include "Polygon.h"
#include "Triangle.h"
#include "PhongBlinn.h"
#include "Phong.h"
#include "Checkerboard.h"
#include "TRWard.h"
#include "TRReinhard.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <math.h>

using namespace std;

#define print(x) std::cout << x << std::endl;

float kC = 1.0f / 3.0f;

std::vector<Triangle> readObj(std::string fileName)
{
	std::vector<Point> vertices;
	std::vector<Triangle> obj;

	std::ifstream inFile(fileName);
	std::string line;
	
	while (std::getline(inFile, line))
	{
		istringstream iss(line);
		vector<string> tokens{ istream_iterator<string>{iss},
					  istream_iterator<string>{} };
		if (!tokens.size() < 1)
		{
			if (tokens[0] == "v")
			{
				vertices.push_back(Point(stof(tokens[1]), stof(tokens[2]), stof(tokens[3])));
			}
			else if (tokens[0] == "vn" || tokens[0] == "vt")
			{
				vertices.push_back(Point(0, 0, 0));
			}
			else if (tokens[0] == "f")
			{
				Color c = Color(0.7f, 0.7f, 0.7f);
				Color cSpec = Color(1, 1, 1);
				Material* mt = new Phong(c, cSpec, 0.0f, 0.75f, 0.25f, 100.0f);

				std::string delimiter = "/";
				int p0 = stoi(tokens[1].substr(0, tokens[1].find(delimiter)));
				int p1 = stoi(tokens[2].substr(0, tokens[2].find(delimiter)));
				int p2 = stoi(tokens[3].substr(0, tokens[3].find(delimiter)));

				p0 = (p0 < 0) ? -1 * p0 : p0;
				p1 = (p1 < 0) ? -1 * p1 : p1;
				p2 = (p2 < 0) ? -1 * p2 : p2;

				obj.push_back(Triangle(mt, vertices[p0 - 1], vertices[p1 - 1], vertices[p2 - 1]));
			}
		}
	}
	
	return obj;
}

void createWorld()
{
	// Glass Sphere
	Color colorGlass = Color(0, 1, 0);
	Color specColorGlass = Color(1, 1, 1);
	Phong matGlass = Phong(colorGlass, specColorGlass, 0.0f, 0.75f, 0.25f, 100.0f);
	Point centerGlass = Point(0.0f, 1.0f, 0.0f);
	Point centerGlass2 = Point(0.0f, 3.5f, 0.0f);
	Point centerGlass3 = Point(-2.0f, 2.25f, 0.0f);
	float radiusGlass = 1.0f;

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
	scalingMatrix.row(0) << 1, 0, 0, 0.0;
	scalingMatrix.row(1) << 0, 1, 0, 0.0;
	scalingMatrix.row(2) << 0, 0, 1, 0.0;
	scalingMatrix.row(3) << 0, 0, 0, 1;

	// Create Objects
	Material* m1 = &matGlass;
	Material* m4 = &matTri;
	Material* m5 = &matTri2;

	Sphere sphereGlass = Sphere(m1, centerGlass, radiusGlass);
	Sphere s2 = Sphere(m1, centerGlass2, radiusGlass);
	Sphere s3 = Sphere(m1, centerGlass3, radiusGlass);
	Triangle t1 = Triangle(m4, p0, p1, p2);
	Triangle t2 = Triangle(m5, p3, p4, p5);

	// Wrap objects in object pointers to induce polymorphism
	Object* o1 = &sphereGlass;
	Object* o2 = &s2;
	Object* o3 = &s3;
	Object* o4 = &t1;
	Object* o5 = &t2;

	// Create LightSources
	Point lightPoint1 = Point(-6.0f, 3.0f, 0.0f);
	LightSource l1 = LightSource(lightPoint1, Color(1, 1, 1));

	// Add objects to world
	World world = World(Color(0.11765f, 0.56471f, 1));

	int glassIndex = world.add(o1);
	world.add(o2);
	world.add(o3);
	world.add(o4);
	world.add(o5);

	// Translate object
	//world.transform(glassIndex, scalingMatrix);

	// Add lights to world
	int light1Index = world.addLight(&l1);

	// Define camera 
	Camera c = Camera(Point(0.0f, 2.5f, -9.0f), Vector3f(0, 0.0f, 0), Vector3f(0, 1, 0));
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
	Color colorGlass = Color(0.11765f, 0.56471f, 1.0f);
	Color specColorGlass = Color(1, 1, 1);
	Phong matGlass = Phong(0.0f, 0.40f, 1.05f, colorGlass * kC, specColorGlass, 0.0f, 0.75f, 0.25f, 100.0f);
	Point centerGlass = Point(-0.121f, 1.5f, -4.724f);
	float radiusGlass = 1.2988f;

	// Mirror Sphere
	Color colorMirror = Color(0.11765f, 0.56471f, 1.0f);
	Color specColorMirror = Color(1, 1, 1);
	Phong matMirror = Phong(0.0f, 0.95f, 1.05f, colorMirror * kC, specColorMirror, 0.0f, 0.70f, 0.30f, 10.0f);
	//Point centerMirror = Point(1.5f, 0.2f, -3.94f);
	Point centerMirror = Point(1.5f, 0.7f, -2.94f);
	float radiusMirror = 1.11f;

	//Floor
	Color colorFloor = Color(1, 0, 0);
	Color specColorFloor = Color(1, 1, 1);
	Phong matFloor = Phong(colorFloor, specColorFloor, 0.0f, 1.0f, 0.0f, 1.0f);
	std::vector<Point> floorVertices{Point(-0.85f, -0.5f, 1), Point(-1, -0.5f, 10), Point(2, -0.5f, 10), Point(2, -0.5f, 1)};

	//Checker Board Triangles
	Color color1 = Color(1, 1, 0);
	Color color2 = Color(1, 0, 0);
	Checkerboard matTri = Checkerboard(color1, color2, Point(0,0,0), Point(0,0,1), Point(1,0,1), specColorFloor, 0.0f, 1.0f, 0.0f, 1.0f);
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
	Point lightPoint1 = Point(-2.0f, 6.014f, -10.0f);
	Point lightPoint2 = Point(2.0f, 6.0f, -6.0f);
	LightSource l1 = LightSource(lightPoint1, Color(100, 100, 100));
	LightSource l2 = LightSource(lightPoint2, Color(1, 1, 1));

	// Add objects to world
	World world = World(Color(10 * 0.11765f, 10 * 0.56471f, 10));

	int glassIndex = world.add(o1);
	int mirrorIndex = world.add(o2);
	//int floorIndex = world.add(o3);
	int triIndex1 = world.add(o4);
	int triIndex2 = world.add(o5);

	// Add lights to world
	int light1Index = world.addLight(&l1);
	//int light2Index = world.addLight(&l2);

	TRWard trop = TRWard(300.0f);
	TRReinhard trop2 = TRReinhard(300.0f);
	TROperator* camTrop = &trop2;

	// Define camera

//    0, 0.941f, -10
//    -0.121f, 1.5, -4.724f
	Camera c = Camera(Point(0, 0.941f, -10), Vector3f(0, 0, 1), Vector3f(0, 1, 0), camTrop);
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
	std::vector<Triangle> bunny = readObj("bunny.obj");

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
	c.setFilmPlaneDim(90, (16 / 9.0f));
	c.setImageDim(1920, 1080);
	c.setSpatialDataStructure(1);

	c.render(world);

	//print(world.toString());
	//print(c.toString());

	std::cin.ignore();
}

void createDragonScene()
{
	std::vector<Triangle> bunny = readObj("mori_knob.obj");

	// Rotation Matrix
	Matrix4f rotationMatrix;
	rotationMatrix.row(0) << cosf(3.14159f), 0, sinf(3.14159f), 0;
	rotationMatrix.row(1) << 0, 1, 0, 0;
	rotationMatrix.row(2) << -sinf(3.14159f), 0, cosf(3.14159f), 0;
	rotationMatrix.row(3) << 0, 0, 0, 1;

	// Translation Matrix
	Matrix4f translationMatrix;
	translationMatrix.row(0) << 1, 0, 0, 0;
	translationMatrix.row(1) << 0, 1, 0, 0;
	translationMatrix.row(2) << 0, 0, 1, 0;
	translationMatrix.row(3) << 0, 0, 0, 1;

	//Scaling Matrix
	Matrix4f scalingMatrix;
	scalingMatrix.row(0) << 1, 0, 0, 0;
	scalingMatrix.row(1) << 0, 1, 0, 0;
	scalingMatrix.row(2) << 0, 0, 1, 0;
	scalingMatrix.row(3) << 0, 0, 0, 1;

	Matrix4f transform = translationMatrix * rotationMatrix * scalingMatrix;

	// Create LightSources
	Point lightPoint1 = Point(-1.0f, 0.014f, -40.0f);
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
	c.setFilmPlaneDim(90, (16 / 9.0f));
	c.setImageDim(1920, 1080);
	c.setSpatialDataStructure(1);

	c.render(world);

	//print(world.toString());
	//print(c.toString());

	std::cin.ignore();
}

int main(void)
{
	createDragonScene();
	//createBunnyScene();
	//createScene1();
	//createWorld();
	return 0;
}