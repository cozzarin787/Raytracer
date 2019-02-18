#pragma once
#include "Point.h"
#include "Color.h"
class LightSource
{
public:
	Point position;
	Color color;

	LightSource(Point position, Color c);
	~LightSource();
};

