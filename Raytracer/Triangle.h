#pragma once
#include "Point.h"
#include "Object.h"

class Triangle : public Object
{
public:
	Point p0;
	Point p1;
	Point p2;

	Triangle(Material m, Point p0, Point p1, Point p2);

	intersectResult intersect(Ray r);
	void transform(Matrix4f transMat);

	std::string toString();

	~Triangle();
};

