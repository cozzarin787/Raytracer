#pragma once
#include <string>
#include <Eigen/Dense>

using Eigen::Vector3f; 
using Eigen::Vector4f;

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
		Vector4f homogen();

		// To String
		std::string toString();

		// Destructors
		~Point();
};

