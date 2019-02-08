#pragma once
#include <string>
#include <Eigen/Dense>

using Eigen::Vector3f;

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
		Vector3f vector();

		// To String
		std::string toString();

		// Destructors
		~Point();
};

