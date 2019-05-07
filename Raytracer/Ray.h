#pragma once
#include "Point.h"
#include <Eigen/Dense>

using Eigen::Vector3f;

class Ray
{
	public:
		Point origin;
		Vector3f direction;
		Vector3f invDir;
		int invSign[3];

		Ray();
		Ray(Point origin, RowVector3f directions);

		std::string toString();

		~Ray();
};

