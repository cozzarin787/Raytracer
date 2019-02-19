#pragma once
#include "Point.h"
#include <vector>
#include "LightSource.h"

class IntersectData
{
public:
	Point P;							// Point of Intersection
	RowVector3f N;						// Normal Vector
	RowVector3f S;						// Direction of incoming light
	RowVector3f R;						// Perfect Mirror Reflection
	RowVector3f H;						// Halfway Between Viewing Dir and Light Dir
	RowVector3f V;						// Viewing Direction
	std::vector<LightSource> lights;	// List of Light Sources

	IntersectData(Point p, RowVector3f normal, RowVector3f lightDir, RowVector3f view, std::vector<LightSource> lightList);

	~IntersectData();
};

