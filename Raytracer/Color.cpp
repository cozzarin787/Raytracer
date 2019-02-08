#include "Color.h"

Color::Color()
{
	this->r = 1.0f;
	this->g = 1.0f;
	this->b = 1.0f;
}

Color::Color(float r, float g, float b)
{
	this->r = r;
	this->g = g;
	this->b = b;
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
