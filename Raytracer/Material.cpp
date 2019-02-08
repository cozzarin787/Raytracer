#include "Material.h"

Material::Material()
{
	this->color = Color();
}

Material::Material(Color c)
{
	this->color = c;
}

std::string Material::toString()
{
	return std::string(color.toString());
}


Material::~Material()
{
}
