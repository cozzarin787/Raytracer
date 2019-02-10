#pragma once
#include "Object.h"
#include "Ray.h"

class Sphere : public Object
{
public:
	Point center;
	float radius;
	float radius_sqr;

	Sphere(Material mat, Point center, float radius);

	intersectResult intersect(Ray r);

	std::string toString();

	~Sphere();
};

