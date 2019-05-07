#include "Phong.h"

Phong::Phong(Color objectColor, Color specColor, float ambient, float diffuse, float specular, float specHighlight) : Material()
{
	this->k_a = ambient;	// ambient
	this->k_d = diffuse;	// diffuse
	this->k_s = specular;	// specular
	this->k_e = specHighlight;	// spec highlight
	this->C_o = objectColor;  // object color
	this->C_s = specColor;  // specular color
}

Phong::Phong(float kr, float kt, Color objectColor, Color specColor, float ambient, float diffuse, float specular, float specHighlight) : Material(kr, kt)
{
	this->k_a = ambient;	// ambient
	this->k_d = diffuse;	// diffuse
	this->k_s = specular;	// specular
	this->k_e = specHighlight;	// spec highlight
	this->C_o = objectColor;  // object color
	this->C_s = specColor;  // specular color
}

Phong::Phong(float kr, float kt, float ni, Color objectColor, Color specColor, float ambient, float diffuse, float specular, float specHighlight) : Material(kr, kt, ni)
{
	this->k_a = ambient;	// ambient
	this->k_d = diffuse;	// diffuse
	this->k_s = specular;	// specular
	this->k_e = specHighlight;	// spec highlight
	this->C_o = objectColor;  // object color
	this->C_s = specColor;  // specular color
}

Color Phong::illuminate(IntersectData interData)
{

	std::vector<Vector3f> L;
	for (int i = 0; i < interData.lights.size(); i++)
	{
		L.push_back(interData.lights[i]->color.vector());
	}

	// Ambient
	Vector3f ambient = this->k_a * this->C_o.vector().array() * interData.ambientLight.vector().array();
	
	Vector3f diffuse = Vector3f(0, 0, 0);
	Vector3f specular = Vector3f(0, 0, 0);

	// If you can see light from point of intersection:
	if (!interData.lights.empty())
	{
		// Enumerate through all light direct light sources
		for (int i = 0; i < interData.lights.size(); i++)
		{
			// Diffuse
			Vector3f L_iC_o = L[i].array() * this->C_o.vector().array();
			float diffuseDot = interData.S[i].dot(interData.N);
			diffuse += L_iC_o * ((diffuseDot < 0) ? 0.0f : diffuseDot);

			// Specular
			Vector3f L_iC_s = L[i].array() * this->C_s.vector().array();
			float specularDot = interData.R[i].direction.dot(interData.N);
			specular += (L_iC_s * pow((specularDot < 0) ? 0.0f : specularDot, this->k_e));
		}
		diffuse = this->k_d * diffuse;
		specular = this->k_s * specular;
	}
	Vector3f totalRad = ambient + diffuse + specular;
	return Color(totalRad[0], totalRad[1], totalRad[2]);
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
