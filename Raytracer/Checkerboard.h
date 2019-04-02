#pragma once
#include "Material.h"

class Checkerboard : public Material
{
private:
	Color color1; // object color1
	Color color2; // object color2

	Point triU;
	Point triV;
	Point triW;

	float k_a;	// ambient
	float k_d;	// diffuse
	float k_s;	// specular
	float k_e;	// spec highlight
	Color C_s;  // specular color
public:
	Point p0;
	Point p1;
	Point p2;

	Checkerboard(Color c1, Color c2, Point triU, Point triV, Point triW, Color specColor, float ambient, float diffuse, float specular, float specHighlight);

	Color illuminate(IntersectData interData);
	std::string toString();

	~Checkerboard();
};

