#include "Color.h"

Color::Color()
{
	this->r = 255;
	this->g = 255;
	this->b = 255;
	this->a = 255;
}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
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
