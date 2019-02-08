#include "Triangle.h"

Triangle::Triangle(Material m, Point p0, Point p1, Point p2) : Object(m)
{
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
}

float Triangle::intersect(Ray r)
{
	// TODO
	return 0.0f;
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
