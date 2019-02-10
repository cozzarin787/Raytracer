#pragma once
#include <string>

class Color
{
	public:
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;

		Color();
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

		std::string toString();

		~Color();
};

