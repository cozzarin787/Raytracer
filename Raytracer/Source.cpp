#include "Camera.h"
#include <iostream>

int main()
{
	Camera c = Camera(Point(0, 0.941, -10), RowVector3f(0, 0, 1), RowVector3f(0, 1, 0));
	c.setFocalLength(50);
	c.setFilmPlaneDim(60, (4 / 3.0f));
	c.setImageDim(1028, 770);

	std::cout << c.toString() << std::endl;
	std::cin.ignore();
	return 0;
}