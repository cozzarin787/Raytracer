#pragma once
#include <string>

class Color
{
	public:
		float r;
		float g;
		float b;

		Color();
		Color(float r, float g, float b);

		std::string toString();

		~Color();
};

