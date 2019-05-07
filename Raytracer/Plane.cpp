#include "Plane.h"
#include "Ray.h"
#include "Object.h"

Plane::Plane()
{
}

Plane::Plane(Point p, Vector3f normal)
{
	this->p = p;
	this->normal = normal;
	this->F = normal[0] * p.x - normal[1] * p.y - normal[2] * p.z;
}

void Plane::transform(Matrix4f transMat)
{
	// Transform normal and F of plane
	Vector4f planeHomo = Vector4f(this->normal[0], this->normal[1], this->normal[2], this->F);
	Vector4f planePrimeHomo = transMat * planeHomo;
	this->F = planePrimeHomo[3];
	this->normal = Vector3f(planePrimeHomo[0], planePrimeHomo[1], planePrimeHomo[2]).normalized();
	
	// Transform point on plane
	Vector4f pHomo = this->p.homogen();
	Vector4f pPrimeHomo = transMat * pHomo;
	float w = pPrimeHomo[3];
	this->p = Point(pPrimeHomo[0] / w, pPrimeHomo[1] / w, pPrimeHomo[2] / w);
}

std::string Plane::toString()
{
	std::string p = this->p.toString();
	std::string x = std::to_string(this->normal[0]);
	std::string y = std::to_string(this->normal[1]);
	std::string z = std::to_string(this->normal[2]);
	return std::string("Plane\n Point: " + p + "\n Normal: [" + x + " " + y + " " + " " + z + "]");
}

Plane::~Plane()
{
}
