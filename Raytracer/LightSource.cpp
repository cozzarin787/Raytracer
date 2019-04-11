#include "LightSource.h"

LightSource::LightSource(Point position, Color c)
{
	this->position = position;
	this->color = c;
}

void LightSource::transform(Matrix4f transMat)
{
	// Transform center of sphere
	Vector4f centerHomo = this->position.homogen();
	Vector4f centerPrimeHomo = transMat * centerHomo;
	float w = centerPrimeHomo[3];
	this->position = Point(centerPrimeHomo[0] / w, centerPrimeHomo[1] / w, centerPrimeHomo[2] / w);
}

std::string LightSource::toString()
{
	std::string p = this->position.toString();
	std::string c = this->color.toString();
	return std::string("LightSource\n Position: " + p + "\n Color: " + c + "\n");
}

LightSource::~LightSource()
{
}
