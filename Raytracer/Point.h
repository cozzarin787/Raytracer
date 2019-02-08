#pragma once
#include <string>
class Point
{
	public:
		// Attributes
		float x;
		float y;
		float z;

		// Constructors
		Point();
		Point(float x, float y, float z);

		// Functions
		float distance(Point);
		void transform(float, float, float);

		// To String
		std::string toString();

		// Destructors
		~Point();
};

