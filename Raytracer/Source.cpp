#include "Polygon.h"
#include <iostream>

int main()
{
	std::vector<Point> v { Point(4, 1, 4), Point(4, 1, -4), Point(-4, 1, 4), Point(-4, 1, -4) };
	Polygon o = Polygon(Material(Color()), v);
	std::cout << o.toString() << std::endl;
	std::cin.ignore();
	return 0;
}