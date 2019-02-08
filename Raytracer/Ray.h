#pragma once
#include "Point.h"
#include <Eigen/Dense>

using Eigen::Vector3f;

class Ray
{
	public:
		Point origin;
		Vector3f direction;

		Ray(Point origin, Vector3f directions);

		~Ray();
};

