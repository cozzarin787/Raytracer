#include "Color.h"

Color::Color()
{
	this->r = 255;
	this->g = 255;
	this->b = 255;
	this->a = 255;
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
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
