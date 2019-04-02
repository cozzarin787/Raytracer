#pragma once
#include "Point.h"
#include <Eigen/Dense>

using Eigen::RowVector3f;

class Ray
{
	public:
		Point origin;
		RowVector3f direction;
		RowVector3f invDir;
		int invSign[3];

		Ray(Point origin, RowVector3f directions);

		std::string toString();

		~Ray();
};

