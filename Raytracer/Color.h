#pragma once
#include <string>

class Color
{
	public:
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;

		Color();
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

		std::string toString();

		~Color();
};

