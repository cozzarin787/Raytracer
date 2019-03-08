#include "Voxel.h"



Voxel::Voxel()
{
}

Voxel::Voxel(std::vector<Point> boundingPoints)
{
	this->vertices = boundingPoints;
}

Voxel::intersectVoxel Voxel::intersect(Ray r)
{
	return Voxel::intersectVoxel(Point(0,0,0), Point(0,0,0));
}


Voxel::~Voxel()
{
}
