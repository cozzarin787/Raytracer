#pragma once
#include "Point.h"
#include <vector>
#include "LightSource.h"
#include "Ray.h"

class IntersectData
{
public:
	Point P;							// Point of Intersection
	Vector3f N;						// Normal Vector
	std::vector<Vector3f> S;			// Direction of incoming light
	std::vector<Vector3f> R;			// Perfect Mirror Reflection
	std::vector<Vector3f> H;			// Halfway Between Viewing Dir and Light Dir
	Vector3f V;						// Viewing Direction
	std::vector<LightSource*> lights;	// List of Light Sources
	Color ambientLight;					// Ambient Background of the World

	IntersectData(Point p, Vector3f normal, std::vector<Vector3f> lightDirs, Vector3f view, std::vector<LightSource*> lightList, Color ambientLight);

	Ray reflect(Vector3f normal, Ray r);

	~IntersectData();
};

