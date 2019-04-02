#pragma once
#include <vector>
#include "Point.h"
#include "Ray.h"
#include <Eigen/dense>

using Eigen::RowVector3f;
using Eigen::Matrix;
using Eigen::RowMajor;

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
	void transform(Matrix<float, 4, 4, RowMajor> transMat);
	std::string toString();

	~Voxel();
};

