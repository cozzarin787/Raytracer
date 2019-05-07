#pragma once
#include <vector>
#include <cmath>
#include "Point.h"
#include "LightSource.h"
#include "Ray.h"

class IntersectData
{
public:
	Point P;							// Point of Intersection
	Vector3f N;						// Normal Vector
	std::vector<Vector3f> S;			// Direction of light vectors
	std::vector<Ray> R;					// Perfect Mirror Reflection
	Ray T;                              // Refraction Ray
	std::vector<Vector3f> H;			// Halfway Between Viewing Dir and Light Dir
	Vector3f V;						// Viewing Direction
	std::vector<LightSource*> lights;	// List of Light Sources
	Color ambientLight;					// Ambient Background of the World

	IntersectData(Point p, Vector3f normal, std::vector<Vector3f> lightDirs, Vector3f view,
	        std::vector<LightSource*> lightList, Color ambientLight, float ni, float nt);

	Ray reflect(Vector3f normal, Ray r);
    Ray refract(Vector3f normal, Ray r, float ni, float nt);

	~IntersectData();
};

