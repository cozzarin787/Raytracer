#pragma once
#include "Point.h"

using Eigen::Matrix4f;

class Plane
{
public:
	Point p;
	Vector3f normal;
	float F;

	Plane();
	Plane(Point p, Vector3f normal);

	void transform(Matrix4f transMat);

	std::string toString();

	~Plane();
};

