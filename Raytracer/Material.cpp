#include "Material.h"

Material::Material()
{
}

Material::Material(float kr, float kt)
{
	if (kr + kt <= 1)
	{
		this->kr = kr;
		this->kt = kt;
	}
	else
	{
		this->kr = 0;
		this->kt = 0;
	}
}


Material::~Material()
{
}
