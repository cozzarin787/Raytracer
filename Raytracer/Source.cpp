#include "Ray.h"
#include <iostream>
#include <Eigen/Dense>

int main()
{
	Ray R = Ray(Point(0, 0, 0), Vector3f(1, 0, 0));
	std::cout << R.toString() << std::endl;
	std::cin.ignore();
	return 0;
}