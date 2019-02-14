#include "World.h"

World::World()
{
}

int World::add(Object* o)
{
	this->objectList.push_back(o);
	return this->objectList.size() - 1;
}

void World::transform(int index, Matrix<float, 4, 4, RowMajor> transMat)
{
	objectList[index]->transform(transMat);
}

void World::transformAllObjects(Matrix<float, 4, 4, RowMajor> transMat)
{
	for (Object* o : this->objectList)
	{
		o->transform(transMat);
	}
}

std::vector<Object::intersectResult> World::spawnRay(Ray r)
{
	std::vector<Object::intersectResult> retVal;
	for (int i = 0; i < this->objectList.size(); i++)
	{
		Object::intersectResult ir = objectList[i]->intersect(r);
		if (ir.isIntersection)
		{
			retVal.push_back(ir);
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
		o_list += "   object" + str_i + "\n      " + this->objectList[i]->toString();
	}
	return std::string("World\n" + o_list);
}

World::~World()
{
}
