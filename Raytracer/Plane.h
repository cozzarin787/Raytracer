#pragma once
#include "Point.h"

using Eigen::Matrix;
using Eigen::RowMajor;

class Plane
{
public:
	Point p;
	RowVector3f normal;
	float F;

	Plane();
	Plane(Point p, RowVector3f normal);

	void transform(Matrix<float, 4, 4, RowMajor> transMat);

	std::string toString();

	~Plane();
};

