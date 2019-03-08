#include "Voxel.h"



Voxel::Voxel()
{
}

Voxel::Voxel(Point min, Point max)
{
	this->min = min;
	this->max = max;
}

Voxel::intersectVoxel Voxel::intersect(Ray r)
{
	return Voxel::intersectVoxel(Point(0,0,0), Point(0,0,0));
}

bool Voxel::inVoxel(Voxel v)
{
	for (int i = 0; i < 3; i++)
	{
		if ((v.min.vector()[i] > this->max.vector()[i]) || (this->min.vector()[i] > v.max.vector()[i]))
		{
			return false;
		}

		return true;
	}
}

Voxel::~Voxel()
{
}
