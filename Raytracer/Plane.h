#pragma once
#include "Point.h"
#include "Object.h"

class Plane : public Object
{
public:
	Point p;
	Vector3f normal;

	Plane(Material m, Point p, Vector3f normal);

	float intersect(Ray r);

	std::string toString();

	~Plane();
};

