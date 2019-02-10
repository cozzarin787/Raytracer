#pragma once
#include "Point.h"
#include "Object.h"

class Plane : public Object
{
public:
	Point p;
	Vector3f normal;
	float F;

	Plane(Material m, Point p, Vector3f normal);

	intersectResult intersect(Ray r);

	std::string toString();

	~Plane();
};

