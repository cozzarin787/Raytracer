#pragma once
#include "Color.h"
#include "Ray.h"
#include <string>

class Material
{
public:
	Color color;

	Material();
	Material(Color c);

	std::string toString();

	~Material();
};

