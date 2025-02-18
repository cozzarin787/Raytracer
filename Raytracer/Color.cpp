#include "Color.h"

Color::Color()
{
	this->r = 1;
	this->g = 1;
	this->b = 1;
}

Color::Color(float r, float g, float b)
{
	this->r = r;
	this->g = g;
	this->b = b;
}

Vector3f Color::vector()
{
	return Vector3f(this->r, this->g, this->b);
}

Color Color::operator+(const Color& other)
{
	return Color(r + other.r, g + other.g, b + other.b);
}
Color Color::operator*(const float k)
{
	return Color(r * k, g * k, b * k);
}
Color Color::operator/(const float k)
{
    return Color(r / k, g / k, b / k);
}
std::string Color::toString()
{
	std::string red = std::to_string(this->r);
	std::string green = std::to_string(this->g);
	std::string blue = std::to_string(this->b);
	return std::string("r: " + red + " g: " + green + " b: " + blue);
}

Color::~Color()
{
}
