#pragma once
#include "Point.h"
#include "Object.h"

using Eigen::Matrix;
using Eigen::RowMajor;

class Triangle : public Object
{
public:
	Point p0;
	Point p1;
	Point p2;

	Triangle(Material* m, Point p0, Point p1, Point p2);

	intersectResult intersect(Ray r);
	void transform(Matrix<float, 4, 4, RowMajor> transMat);

	std::string toString();

	~Triangle();
};

