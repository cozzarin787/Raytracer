#include "Camera.h"
#include <iostream>

int main()
{
	Camera c = Camera(Point(0, 4, -10), Vector3f(0, 0, 1), Vector3f(0, 1, 0));
	std::cout << c.toString() << std::endl;
	std::cin.ignore();
	return 0;
}