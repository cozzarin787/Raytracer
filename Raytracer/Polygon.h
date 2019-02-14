#pragma once
#include "Object.h"
#include "Ray.h"
#include <vector>

using Eigen::Matrix;
using Eigen::RowMajor;

class Polygon : public Object
{
public:
	std::vector<Point> vertices;
	RowVector3f normal;
	float F;

	Polygon(Material mat, std::vector<Point> v_list);
	
	intersectResult intersect(Ray r);
	void transform(Matrix<float, 4, 4, RowMajor> transMat);

	std::string toString();

	~Polygon();
};

