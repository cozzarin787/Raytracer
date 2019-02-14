#pragma once
#include "Object.h"
#include "Ray.h"

using Eigen::Matrix;
using Eigen::RowMajor;

class Sphere : public Object
{
public:
	Point center;
	float radius;
	float radius_sqr;

	Sphere(Material mat, Point center, float radius);

	intersectResult intersect(Ray r);
	void transform(Matrix<float, 4, 4, RowMajor> transMat);

	std::string toString();

	~Sphere();
};

