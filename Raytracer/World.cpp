#include "World.h"

World::World()
{
}

void World::add(Object o)
{
	this->objectList.push_back(o);
}

void World::transform(Object o, Matrix4f transMat)
{
	// TODO
}

void World::transformAllObjects(Matrix4f transMat)
{
	// TODO
}

std::vector<Object::intersectResult> World::spawnRay(Ray r)
{
	std::vector<Object::intersectResult> retVal;
	for (int i = 0; i < this->objectList.size(); i++)
	{
		Object::intersectResult intersectTest = objectList[i].intersect(r);
		if (intersectTest.isIntersection)
		{
			retVal.push_back(intersectTest);
		}
	}
	return retVal;
}

std::string World::toString()
{
	std::string o_list = "Object List\n";
	for (int i = 0; i < this->objectList.size(); i++)
	{
		std::string str_i = std::to_string(i);
		o_list += "   object" + str_i + "\n      " + this->objectList[i].toString();
	}
	return std::string("World\n" + o_list);
}

World::~World()
{
}
