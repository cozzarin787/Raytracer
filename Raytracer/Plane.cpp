#include "Plane.h"
#include "Ray.h"

Plane::Plane(Material* m, Point p, RowVector3f normal) : Object(m)
{
	this->p = p;
	this->normal = normal;
	this->F = normal[0] * p.x - normal[1] * p.y - normal[2] * p.z;
}

Object::intersectResult Plane::intersect(Ray r)
{
	float denom = this->normal[0] * r.direction[0] + this->normal[1] * r.direction[1] + this->normal[2] * r.direction[2];
	if (denom <= 0)
	{
		return intersectResult(false);
	}
	
	float numan = -1 * (this->normal[0] * r.origin.x + this->normal[1] * r.origin.y + this->normal[2] * r.origin.z + F);
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

	return intersectResult(true, omega, this->mat, i, this->normal);
}

void Plane::transform(Matrix<float, 4, 4, RowMajor> transMat)
{
	// Transform normal and F of plane
	RowVector4f planeHomo = RowVector4f(this->normal[0], this->normal[1], this->normal[2], this->F);
	RowVector4f planePrimeHomo = transMat * planeHomo.transpose();
	this->F = planePrimeHomo[3];
	this->normal = RowVector3f(planePrimeHomo[0], planePrimeHomo[1], planePrimeHomo[2]).normalized();
	
	// Transform point on plane
	RowVector4f pHomo = this->p.homogen();
	RowVector4f pPrimeHomo = pHomo * transMat;
	float w = pPrimeHomo[3];
	this->p = Point(pPrimeHomo[0] / w, pPrimeHomo[1] / w, pPrimeHomo[2] / w);
}

std::string Plane::toString()
{
	std::string p = this->p.toString();
	std::string x = std::to_string(this->normal[0]);
	std::string y = std::to_string(this->normal[1]);
	std::string z = std::to_string(this->normal[2]);
	std::string m = this->mat->toString();
	return std::string("Plane\n Point: " + p + "\n Normal: [" + x + " " + y + " " + " " + z + "]");
}

Plane::~Plane()
{
}
