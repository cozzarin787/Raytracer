#include "Plane.h"
#include "Ray.h"
#include "Object.h"

Plane::Plane()
{
}

Plane::Plane(Point p, RowVector3f normal)
{
	this->p = p;
	this->normal = normal;
	this->F = normal[0] * p.x - normal[1] * p.y - normal[2] * p.z;
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
	return std::string("Plane\n Point: " + p + "\n Normal: [" + x + " " + y + " " + " " + z + "]");
}

Plane::~Plane()
{
}
