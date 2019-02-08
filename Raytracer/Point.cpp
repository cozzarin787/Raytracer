#include "Point.h"
#include <math.h>

Point::Point()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Point::Point(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float Point::distance(Point)
{
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

void Point::transform(float x, float y, float z)
{
	this->x += x;
	this->y += y;
	this->z += z;
}

Vector3f Point::vector()
{
	return Vector3f(this->x, this->y, this->z);
}

std::string Point::toString()
{
	std::string x = std::to_string(this->x);
	std::string y = std::to_string(this->y);
	std::string z = std::to_string(this->z);
	return std::string("(" + x + ", " + y + ", " + z + ")");
}

Point::~Point()
{

}
