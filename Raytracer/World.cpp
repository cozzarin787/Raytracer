#include "World.h"

World::World()
{
	this->background = Color(0, 0, 0);
}

World::World(Color background)
{
	this->background = background;
}

int World::add(Object* o)
{
	// Calculate voxel around object


	this->objectList.push_back(o);
	return (int) this->objectList.size() - 1;
}

int World::addLight(LightSource light)
{
	this->lightList.push_back(light);
	return (int) this->lightList.size() - 1;
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

void World::transformAllLights(Matrix<float, 4, 4, RowMajor> transMat)
{
	for (LightSource l : this->lightList)
	{
		l.transform(transMat);
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
