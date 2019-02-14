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
	RowVector3f 
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

	RowVector3f vec = RowVector3f(
		Q.dot(e2),
		P.dot(T),
		Q.dot(r.direction));

	vec *= (1 / denom);
	omega = vec[0];
	u = vec[1];
	v = vec[2];
	w = 1 - u - v;
	if (w < 0)
	{
		// intersection behind camera
		return intersectResult(false);
	}
	else if (u < 0 || v < 0 || u + v > 1)
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
		RowVector3f normal = e1.cross(e2).normalized();

		return intersectResult(true, omega, this->mat);
	}
}

void Triangle::transform(Matrix<float, 4, 4, RowMajor> transMat)
{
	// Transform p0
	RowVector4f p0Homo = this->p0.homogen();
	RowVector4f p0PrimeHomo = p0Homo * transMat;
	float w = p0PrimeHomo[3];
	this->p0 = Point(p0PrimeHomo[0] / w, p0PrimeHomo[1] / w, p0PrimeHomo[2] / w);

	// Transform p1
	RowVector4f p1Homo = this->p1.homogen();
	RowVector4f p1PrimeHomo = p1Homo * transMat;
	w = p1PrimeHomo[3];
	this->p1 = Point(p1PrimeHomo[0] / w, p1PrimeHomo[1] / w, p1PrimeHomo[2] / w);

	// Transform p2
	RowVector4f p2Homo = this->p2.homogen();
	RowVector4f p2PrimeHomo = p2Homo * transMat;
	w = p2PrimeHomo[3];
	this->p2 = Point(p2PrimeHomo[0] / w, p2PrimeHomo[1] / w, p2PrimeHomo[2] / w);
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
