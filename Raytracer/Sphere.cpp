#include "Sphere.h"

Sphere::Sphere(Material mat, Point center, float radius) : Object(mat)
{
	this->center = center;
	this->radius = radius;
	this->radius_sqr = radius*radius;
};

Object::intersectResult Sphere::intersect(Ray r)
{
	//Origin of sphere
	float xc = this->center.x;
	float yc = this->center.y;
	float zc = this->center.z;

	//Origin of ray
	float x0 = r.origin.x;
	float y0 = r.origin.y;
	float z0 = r.origin.z;

	//Direction
	float dx = r.direction[0];
	float dy = r.direction[1];
	float dz = r.direction[2];
	
	float A = 1;
	float B = 2 * (dx*(x0 - xc) + dy*(y0 - yc) + dz*(z0 - zc));
	float C = (x0 - xc)*(x0 - xc) + (y0 - yc)*(y0 - yc) + (z0 - zc)*(z0 - zc) - this->radius_sqr;

	float roots = sqrt(pow(B,2)-4*A*C);
	float omega = omega = (-B + roots) / 2;
	float xi, yi, zi;
	
	Point intersecpoints[2];
	Vector3f normals[2];

	if (roots < 0)
	{
		// no real root, no intersection
		return intersectResult(false);
	}
	else if (roots == 0) 
	{ 
		// one root, ray intersects at sphere's surface
		xi = x0 + dx * omega;
		yi = y0 + dy * omega;
		zi = z0 + dz * omega;
		intersecpoints[0] = Point(xi, yi, zi);
	}
	else
	{
		// two roots, ray goes through sphere
		for (int i = 0; i < 2; i++) 
		{
			xi = x0 + dx * omega;
			yi = y0 + dy * omega;
			zi = z0 + dz * omega;
			intersecpoints[i] = Point(xi, yi, zi);
			omega *= -1;
		}
	}
	for (Point p : intersecpoints)
	{
		int i = 0;
		normals[i++] = Vector3f(xi - xc, yi - yc, zi - zc);
	}
	return intersectResult(true, omega, this->mat);
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
