#pragma once
#include "Point.h"
#include "Object.h"

using Eigen::Matrix;
using Eigen::RowMajor;

class Plane : public Object
{
public:
	Point p;
	RowVector3f normal;
	float F;

	Plane(Material* m, Point p, RowVector3f normal);

	intersectResult intersect(Ray r);
	void transform(Matrix<float, 4, 4, RowMajor> transMat);

	std::string toString();

	~Plane();
};

