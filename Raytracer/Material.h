#pragma once
#include "Color.h"
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

