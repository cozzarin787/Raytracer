#pragma once
#include <string>
#include <Eigen/Dense>

using Eigen::RowVector3f;

class Color
{
	public:
		float r;
		float g;
		float b;

		Color();
		Color(float r, float g, float b);

		RowVector3f vector();

		std::string toString();

		~Color();
};

