#pragma once
#include "Point.h"
#include "Object.h"

using Eigen::Matrix4f;

class Triangle : public Object
{
public:
	Point p0;
	Point p1;
	Point p2;

	Triangle(Material* m, Point p0, Point p1, Point p2);

	intersectResult intersect(Ray r);
	void transform(Matrix4f transMat);
	bool inVoxel(Voxel v);

	std::string toString();

	~Triangle();
};

