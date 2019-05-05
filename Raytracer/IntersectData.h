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
	RowVector3f N;						// Normal Vector
	std::vector<RowVector3f> S;			// Direction of light vectors
	std::vector<Ray> R;					// Perfect Mirror Reflection
	Ray T;                              // Refraction Ray
	std::vector<RowVector3f> H;			// Halfway Between Viewing Dir and Light Dir
	RowVector3f V;						// Viewing Direction
	std::vector<LightSource> lights;	// List of Light Sources
	Color ambientLight;					// Ambient Background of the World
    bool totalInternalReflection;

	IntersectData(Point p, RowVector3f normal, std::vector<RowVector3f> lightDirs, RowVector3f view,
	        std::vector<LightSource> lightList, Color ambientLight, float ni, float nt);

	Ray reflect(RowVector3f normal, Ray r);
    Ray refract(RowVector3f normal, Ray r, float ni, float nt);

	~IntersectData();
};

