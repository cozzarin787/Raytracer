#pragma once
#include "Object.h"
#include "Ray.h"

using Eigen::Matrix4f;

class Sphere : public Object
{
public:
	Point center;
	float radius;
	float radius_sqr;

	Sphere(Material* mat, Point center, float radius);

	intersectResult intersect(Ray r);
	void transform(Matrix4f transMat);
	bool inVoxel(Voxel v);

	std::string toString();

	~Sphere();
};

