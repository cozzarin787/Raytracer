#include "LightSource.h"

LightSource::LightSource(Point position, Color c)
{
	this->position = position;
	this->color = c;
}

LightSource::~LightSource()
{
}
