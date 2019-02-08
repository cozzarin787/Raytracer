#include "Triangle.h"
#include <iostream>

int main()
{
	Triangle o = Triangle(Material(Color()), Point(0,0,0), Point(1, 0, 0), Point(2, 0, 0));
	std::cout << o.toString() << std::endl;
	std::cin.ignore();
	return 0;
}