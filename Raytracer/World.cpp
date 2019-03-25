#include "World.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Polygon.h"

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
	Point min = Point(0, 0, 0);
	Point max = Point(0, 0, 0);
	
	// Check which type the object is
	if (dynamic_cast<Triangle*>(o))
	{
		Triangle* t = dynamic_cast<Triangle*>(o);

		// Calculate voxel dimensions of Triangle
		min.x = t->p0.x;
		min.y = t->p0.y;
		min.z = t->p0.z;

		max.x = t->p0.x;
		max.y = t->p0.y;
		max.z = t->p0.z;

		std::vector<Point> vertices = { t->p0, t->p1, t->p2 };

		for (int i = 0; i < 2; i++)
		{
			if (vertices[i].x < min.x) min.x = vertices[i].x;
			if (vertices[i].y < min.y) min.y = vertices[i].y;
			if (vertices[i].z < min.z) min.z = vertices[i].z;
			if (vertices[i].x > max.x) max.x = vertices[i].x;
			if (vertices[i].y > max.y) max.y = vertices[i].y;
			if (vertices[i].z > max.z) max.z = vertices[i].z;
		}
	}
	else if (dynamic_cast<Sphere*>(o))
	{
		Sphere* s = dynamic_cast<Sphere*>(o);

		// Calculate voxel dimensions of Sphere
		min.x = s->center.x - s->radius;
		min.y = s->center.y - s->radius;
		min.z = s->center.z - s->radius;

		max.x = s->center.x + s->radius;
		max.y = s->center.y + s->radius;
		max.z = s->center.z + s->radius;
	}
	else if (dynamic_cast<Polygon*>(o))
	{
		Polygon* p = dynamic_cast<Polygon*>(o);

		// Calculate voxel dimensions of Polygon
		min.x = p->vertices[0].x;
		min.y = p->vertices[0].y;
		min.z = p->vertices[0].z;

		max.x = p->vertices[0].x;
		max.y = p->vertices[0].y;
		max.z = p->vertices[0].z;

		for (int i = 0; i < p->vertices.size(); i++)
		{
			if (p->vertices[i].x < min.x) min.x = p->vertices[i].x;
			if (p->vertices[i].y < min.y) min.y = p->vertices[i].y;
			if (p->vertices[i].z < min.z) min.z = p->vertices[i].z;
			if (p->vertices[i].x > max.x) max.x = p->vertices[i].x;
			if (p->vertices[i].y > max.y) max.y = p->vertices[i].y;
			if (p->vertices[i].z > max.z) max.z = p->vertices[i].z;
		}
	}
	else
	{
		return -1;
	}

	// Create Voxel and wrap the object with it
	Voxel v = Voxel(min, max);
	voxelObjectWrapper vow = voxelObjectWrapper(o, v);

	this->voxelObjectList.push_back(vow);
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
	// Transform voxel wrapping object, as well as the object
	for (voxelObjectWrapper vow : this->voxelObjectList)
	{
		vow.o->transform(transMat);
		vow.v.transform(transMat);
	}

	// Transform objects
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

void World::calcWorldVoxel()
{
	// calculate the min and max points for the bounding voxel of the entire world
	Point min = this->voxelObjectList[0].v.min;
	Point max = this->voxelObjectList[0].v.max;

	for (int i = 0; i < this->voxelObjectList.size(); i++)
	{
		if (this->voxelObjectList[i].v.min.x < min.x) min.x = this->voxelObjectList[i].v.min.x;
		if (this->voxelObjectList[i].v.min.y < min.y) min.y = this->voxelObjectList[i].v.min.y;
		if (this->voxelObjectList[i].v.min.z < min.z) min.z = this->voxelObjectList[i].v.min.z;
		if (this->voxelObjectList[i].v.max.x > max.x) max.x = this->voxelObjectList[i].v.max.x;
		if (this->voxelObjectList[i].v.max.y > max.y) max.y = this->voxelObjectList[i].v.max.y;
		if (this->voxelObjectList[i].v.max.z > max.z) max.z = this->voxelObjectList[i].v.max.z;
	}
	min.x -= 1.0f;
	min.y -= 1.0f;
	min.z -= 1.0f;
	max.x += 1.0f;
	max.y += 1.0f;
	max.z += 1.0f;
	this->totalBound = Voxel(min, max);
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
