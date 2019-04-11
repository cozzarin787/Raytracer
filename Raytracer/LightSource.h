#pragma once
#include "Point.h"
#include "Color.h"
#include <Eigen/Dense>

using Eigen::Matrix4f;

class LightSource
{
public:
	Point position;
	Color color;

	LightSource(Point position, Color c);

	void transform(Matrix4f transMat);

	std::string toString();

	~LightSource();
};

