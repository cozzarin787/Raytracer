#include "Phong.h"

Phong::Phong(Color objectColor, Color specColor, float ambient, float diffuse, float specular, float specHighlight)
{
	this->k_a = ambient;	// ambient
	this->k_d = diffuse;	// diffuse
	this->k_s = specular;	// specular
	this->k_e = specHighlight;	// spec highlight
	this->C_o = objectColor;  // object color
	this->C_s = specColor;  // specular color
}

RowVector3f Phong::illuminate(IntersectData interData)
{
	RowVector3f L_i = interData.lights[0].color.vector();

	// Diffuse
	RowVector3f diffuse = this->k_d * (L_i * this->C_o.vector() * (interData.S.dot(interData.N)));

	// Specular
	RowVector3f specular = this->k_s * (L_i * this->C_s.vector() * pow((interData.R.dot(interData.V)), this->k_e));

	return diffuse + specular;
}

std::string Phong::toString()
{
	std::string s1 = std::to_string(this->k_a);
	std::string s2 = std::to_string(this->k_d);
	std::string s3 = std::to_string(this->k_s);
	std::string s4 = std::to_string(this->k_e);
	std::string s5 = this->C_o.toString();
	std::string s6 = this->C_s.toString();
	return std::string("Phong:\n Ambient: " + 
						s1 + "\n Diffuse: " +
						s2 + "\n Specular: " +
						s3 + "\n Spec Highlight: " +
						s4 + "\n Object Color: " +
						s5 + "\n Spec Color: " +
						s6 + "\n");
}

Phong::~Phong()
{
}
