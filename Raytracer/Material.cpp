#include "Material.h"

Material::Material()
{
	this->kr = 0;
	this->kt = 0;
	this->indexRefract = 1;
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
	this->indexRefract = 1;
}

Material::Material(float kr, float kt, float ni)
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
	this->indexRefract = ni;
}


Material::~Material()
{
}
