#include "Ray.h"

Ray::Ray()
{
	this->origin = Point(0, 0, 0);
	this->direction = RowVector3f(1, 1, 1);
}

Ray::Ray(Point origin, RowVector3f direction)
{
	this->origin = origin;
	this->direction = direction.normalized();
}

std::string Ray::toString()
{
	std::string x = std::to_string(this->direction[0]);
	std::string y = std::to_string(this->direction[1]);
	std::string z = std::to_string(this->direction[2]);
	return std::string("Ray\n origin: " + origin.toString() + "\n direction: [" + x + " " + y + " " + " " + z + "]");
}

Ray::~Ray()
{
}
