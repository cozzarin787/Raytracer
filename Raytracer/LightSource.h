#pragma once
#include "Point.h"
#include "Color.h"
#include <Eigen/Dense>

using Eigen::Matrix;
using Eigen::RowMajor;

class LightSource
{
public:
	Point position;
	Color color;

	LightSource(Point position, Color c);

	void transform(Matrix<float, 4, 4, RowMajor> transMat);

	std::string toString();

	~LightSource();
};

