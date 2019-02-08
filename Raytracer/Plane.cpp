#include "Plane.h"

Plane::Plane(Material m, Point p, Vector3f normal) : Object(m)
{
	this->p = p;
	this->normal = normal;
}

Object::intersectResult Plane::intersect(Ray r)
{
	// TODO
	return intersectResult();
}

std::string Plane::toString()
{
	std::string p = this->p.toString();
	std::string x = std::to_string(this->normal[0]);
	std::string y = std::to_string(this->normal[1]);
	std::string z = std::to_string(this->normal[2]);
	std::string m = this->mat.toString();
	return std::string("Plane\n Point: " + p + "\n Normal: [" + x + " " + y + " " + " " + z + "]");
}

Plane::~Plane()
{
}
