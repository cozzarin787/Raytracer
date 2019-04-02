#include "Ray.h"

Ray::Ray(Point origin, RowVector3f direction)
{
	this->origin = origin;
	this->direction = direction.normalized();
	this->invDir = -1 * this->direction;
	this->invSign[0] = (this->invDir[0] < 0);
	this->invSign[1] = (this->invDir[1] < 0);
	this->invSign[2] = (this->invDir[2] < 0);
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
