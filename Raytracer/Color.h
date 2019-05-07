#pragma once
#include <string>
#include <Eigen/Dense>

using Eigen::Vector3f;

class Color
{
	public:
		float r;
		float g;
		float b;

		Color();
		Color(float r, float g, float b);

		RowVector3f vector();
		Color operator+(const Color & other);
		Color operator*(const float k);
        Color operator/(const float k);
		std::string toString();

		~Color();
};

