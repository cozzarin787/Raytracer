#include "Sphere.h"

Sphere::Sphere(Material mat, Point center, float radius) : Object(mat)
{
	this->center = center;
	this->radius = radius;
};

float Sphere::intersect(Ray r)
{
	// TODO
	return 0.0f;
}

std::string Sphere::toString()
{
	std::string m = this->mat.toString();
	std::string c = this->center.toString();
	std::string r = std::to_string(this->radius);
	return std::string("Sphere\n Material: " + m + "\n Center: " + c + "\n Radius: " + r);
}

Sphere::~Sphere()
{
}
