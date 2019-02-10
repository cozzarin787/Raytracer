#include "Triangle.h"

Triangle::Triangle(Material m, Point p0, Point p1, Point p2) : Object(m)
{
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
}

Object::intersectResult Triangle::intersect(Ray r)
{
	float omega = 0.0f, u = 0.0f, v = 0.0f, w = 0.0f;
	Vector3f 
		e1 = p1.vector() - p0.vector(),
		e2 = p2.vector() - p0.vector(),
		T = r.origin.vector() - p0.vector(),
		P = r.direction.cross(e2),
		Q = T.cross(e1);

	float denom = P.dot(e1);
	if (denom == 0) 
	{
		return intersectResult(false);
	}

	Vector3f vec = Vector3f(
		Q.dot(e2),
		P.dot(T),
		Q.dot(r.direction));

	vec *= (1 / denom);
	omega = vec[0];
	u = vec[1];
	v = vec[1];
	w = 1 - u - v;

	if (u < 0 && v < 0 && u + v > 1)
	{
		// intersection outside of the triangle
		return intersectResult(false);
	}
	else
	{
		// calc intersection point
		float xi = w * p0.x + u * p1.x + v * p2.x;
		float yi = w * p0.y + u * p1.y + v * p2.y;
		float zi = w * p0.z + u * p1.z + v * p2.z;
		Point i = Point(xi, yi, zi);

		// calc normal
		Vector3f normal = e1.cross(e2).normalized();

		return intersectResult(true, omega, this->mat);
	}
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
