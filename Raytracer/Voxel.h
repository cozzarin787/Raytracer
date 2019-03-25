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
		Point enter;
		Point exit;

		intersectVoxel(Point ent, Point exi)
		{
			enter = ent;
			exit = exi;
		};
	};

	Point min;
	Point max;

	Voxel();
	Voxel(Point min, Point max);

	intersectVoxel intersect(Ray r);
	bool inVoxel(Voxel v);
	void transform(Matrix<float, 4, 4, RowMajor> transMat);
	std::string toString();

	~Voxel();
};

