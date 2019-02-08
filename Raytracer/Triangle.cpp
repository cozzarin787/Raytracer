#include "Triangle.h"

Triangle::Triangle(Material m, Point p0, Point p1, Point p2) : Object(m)
{
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
}

Object::intersectResult Triangle::intersect(Ray r)
{
	// TODO
	float omega = 0.0f, u = 0.0f, v = 0.0f;
	Vector3f 
		e1 = p1.vector - p0.vector,
		e2 = p2.vector - p0.vector,
		T = r.origin.vector - p0.vector,
		P = r.direction.cross(e2),
		Q = T.cross(e1);
	float denom = P.dot(e1);

	if (denom == 0) 
	{
		return intersectResult();
	}

	return intersectResult();
}

std::string Triangle::toString()
{
	std::string p0 = this->p0.toString();
	std::string p1 = this->p1.toString();
	std::string p2 = this->p1.toString();
	std::string m = this->mat.toString();
	return std::string("Triangle\n Material: " + m + "\n p0: " + p0 + "\n p1: " + p1 + "\n p2: " + p2);
}


Triangle::~Triangle()
{
}
