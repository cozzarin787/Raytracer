#pragma once
#include "Point.h"
#include <Eigen/Dense>

using Eigen::RowVector3f;

class Ray
{
	public:
		Point origin;
		RowVector3f direction;

		Ray();
		Ray(Point origin, RowVector3f directions);

		std::string toString();

		~Ray();
};

