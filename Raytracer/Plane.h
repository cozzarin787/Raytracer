#pragma once
#include "Point.h"
#include "Object.h"

class Plane : public Object
{
public:
	Point p;
	RowVector3f normal;
	float F;

	Plane(Material m, Point p, RowVector3f normal);

	intersectResult intersect(Ray r);
	void transform(Matrix4f transMat);

	std::string toString();

	~Plane();
};

