#include "Ray.h"

Ray::Ray(Point origin, Vector3f direction)
{
	this->origin = origin;
	this->direction = direction;
}


Ray::~Ray()
{
}
