#pragma once
#include <vector>
#include "Point.h"
#include "Ray.h"
#include <Eigen/Dense>

using Eigen::Vector3f;
using Eigen::Matrix4f;

class Voxel
{
public:
	struct intersectVoxel
	{
		bool intersect;
		Point enter;
		Point exit;

		intersectVoxel(bool inter)
		{
			intersect = inter;
		};

		intersectVoxel(bool inter, Point ent, Point exi)
		{
			intersect = inter;
			enter = ent;
			exit = exi;
		};
	};

	Point min;
	Point max;
	Point bounds[2];

	Voxel();
	Voxel(Point min, Point max);

	intersectVoxel intersect(Ray r);
	bool inVoxel(Voxel v);
	void transform(Matrix4f transMat);
	std::string toString();

	~Voxel();
};

