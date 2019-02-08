#include "Color.h"

Color::Color()
{
	this->r = 255.0f;
	this->g = 255.0f;
	this->b = 255.0f;
}

Color::Color(float r, float g, float b)
{
	this->r = r;
	this->g = g;
	this->b = b;
}

Color::~Color()
{
}
