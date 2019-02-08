#include "Plane.h"

Plane::Plane(Material m, Point p, Vector3f normal) : Object(m)
{
	this->p = p;
	this->normal = normal;
	this->F = -1 * normal[0] * p.x - normal[1] * p.y - normal[2] * p.z;
}

Object::intersectResult Plane::intersect(Ray r)
{
	float denom = normal[0] * r.direction[0] - normal[1] * r.direction[1] - normal[2] * r.direction[2];
	if (denom <= 0)
	{
		return intersectResult(false);
	}
	
	float numan = normal[0] * r.origin.x - normal[1] * r.origin.y - normal[2] * r.origin.z;
	float omega = numan / denom;
	if (omega <= 0)
	{
		return intersectResult(false);
	}

	// calc intersection point
	float xi = r.origin.x + r.direction[0] * omega;
	float yi = r.origin.y + r.direction[1] * omega;
	float zi = r.origin.z + r.direction[2] * omega;
	Point i = Point(xi, yi, zi);

	return intersectResult(true, omega, this->mat);
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
