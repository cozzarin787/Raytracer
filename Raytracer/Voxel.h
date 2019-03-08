#pragma once
#include <vector>
#include "Point.h"
#include "Ray.h"
#include <Eigen/dense>

using Eigen::RowVector3f;

class Voxel
{
public:
	struct intersectVoxel
	{
		Point enter;
		Point exit;

		intersectVoxel(Point ent, Point exi)
		{
			enter = ent;
			exit = exi;
		};
	};

	std::vector<Point> vertices;

	Voxel();
	Voxel(std::vector<Point> boundingPoints);

	intersectVoxel intersect(Ray r);

	~Voxel();
};

