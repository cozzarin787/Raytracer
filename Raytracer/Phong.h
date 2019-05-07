#pragma once
#include "Material.h"

class Phong : public Material
{
private:
	float k_a;	// ambient
	float k_d;	// diffuse
	float k_s;	// specular
	float k_e;	// spec highlight
	Color C_o;  // object color
	Color C_s;  // specular color

public:
	Phong(Color objectColor, Color specColor, float ambient, float diffuse, float specular, float specHighlight);
	Phong(float kr, float kt, Color objectColor, Color specColor, float ambient, float diffuse, float specular, float specHighlight);
	Phong(float kr, float kt, float ni, Color objectColor, Color specColor, float ambient, float diffuse, float specular, float specHighlight);

	Color illuminate(IntersectData interData);
	std::string toString();

	~Phong();
};

