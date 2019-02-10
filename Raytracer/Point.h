#pragma once
#include <string>
#include <Eigen/Dense>

using Eigen::RowVector3f; 
using Eigen::RowVector4f;

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
		RowVector3f vector();
		RowVector4f homogen();

		// To String
		std::string toString();

		// Destructors
		~Point();
};

