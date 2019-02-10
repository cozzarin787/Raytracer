#include "Object.h"

Object::Object(Material mat)
{
	this->mat = mat;
}

Object::intersectResult Object::intersect(Ray r)
{
	return intersectResult();
}

void Object::transform(Matrix4f transMat)
{
}

std::string Object::toString()
{
	return std::string("Object");
}

Object::~Object()
{
}
