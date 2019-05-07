#include "ThreadCamera.h"
#include "IntersectData.h"
#include "Checkerboard.h"
#include "TRReinhard.h"
#include "ctpl_stl.h"
#include <lodepng.h>
#include <iostream>
#include <chrono>
#include <thread>

#define PI 3.141592654f
#define print(x) std::cout << x << std::endl;
#define MAX_RECUR 4
#define OUTPUT_FILENAME "test.png"

using namespace std::chrono;

ThreadCamera::ThreadCamera(Point p, Vector3f lookat, Vector3f temp)
{
	this->position = p;
	this->lookat = lookat;
	// default values
	this->focalLength = 1;
	this->spatialFlag = 1;

	// construct view transform
	Vector3f n = (lookat - p.vector()).normalized();
	Vector3f u = ((temp.normalized()).cross(n)).normalized();
	Vector3f v = (n.cross(u)).normalized();

	this->viewTransform << u[0], u[1], u[2], -(p.vector().dot(u)),
		v[0], v[1], v[2], -(p.vector().dot(v)),
		n[0], n[1], n[2], -(p.vector().dot(n)),
		0, 0, 0, 1;

	// Set default tone reproduction operator
	this->TRop = new TRReinhard(300);
}

ThreadCamera::ThreadCamera(Point p, Vector3f lookat, Vector3f temp, TROperator* trop)
{
	this->position = p;
	this->lookat = lookat;
	// default values
	this->focalLength = 1;
	this->spatialFlag = 1;

	// construct view transform
	Vector3f n = (lookat - p.vector()).normalized();
	Vector3f u = ((temp.normalized()).cross(n)).normalized();
	Vector3f v = (n.cross(u)).normalized();

	this->viewTransform << u[0], u[1], u[2], -(p.vector().dot(u)),
		v[0], v[1], v[2], -(p.vector().dot(v)),
		n[0], n[1], n[2], -(p.vector().dot(n)),
		0, 0, 0, 1;

	this->TRop = trop;
}

void ThreadCamera::renderParallel(World world)
{

	// transform world into camera coordinates
	world.transformAllObjects(this->viewTransform);

	// transform light sources into camera coordinates
	world.transformAllLights(this->viewTransform);

	// calculate the bounding box of the world
	world.calcWorldVoxel();

	// BUILD K-D TREE
	if (spatialFlag == 1)
	{
		auto start = high_resolution_clock::now();
		this->treeBuilder = KdTreeBuilder();
		this->KDTree = this->treeBuilder.getNode(world.totalBound, world.voxelObjectList, 0);
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);
		print("\nK-D TREE BUILD TIME: ");
		std::string s = std::to_string(duration.count()) + " milliseconds\n\n";
		print(s);
	}

	// init pixelArray
	std::vector<std::vector<Color>> pixelArray(imageHeightPx);
	for (int i = 0; i < imageHeightPx; i++)
		pixelArray[i].resize(imageWidthPx);

	// Calculate pixel height and width
	float pXh = this->filmPlaneHeight / this->imageHeightPx;
	float pXw = this->filmPlaneWidth / (float)this->imageWidthPx;

	float pxY = this->filmPlaneHeight / 2;
	float pxX = -1 * this->filmPlaneWidth / 2;

	// Get number of cores for thread pool
	unsigned int nthreads = std::thread::hardware_concurrency();
	ctpl::thread_pool tp(nthreads);

	// Create the array of pixels representing the rendered image of the world
	auto start = high_resolution_clock::now();
	for (int i = 0; i < this->imageHeightPx; i++)
	{
		for (int j = 0; j < this->imageWidthPx; j++)
		{
			// Spawn Ray through the middle of a pixel
			Point pxpos = Point(pxX + pXw, pxY - pXh, this->focalLength);
			Point cameraOrigin = Point(0, 0, 0);
			Vector3f rayvec = (pxpos.vector() - cameraOrigin.vector()).normalized();
			Ray r = Ray(cameraOrigin, rayvec);

			rayInfo params;
			params.i = i;
			params.j = j;
			params.r = r;
			tp.push([this, &params, &pixelArray, &world](int id) -> void
			{

				std::vector<Object::intersectResult> intersectlist;

				// Calculate Intersections with world objects by sending ray through this->this->KDTree
				if (spatialFlag == 1)
				{
					intersectlist = this->treeBuilder.rayThroughTree(this->KDTree, params.r);
				}
				else
				{
					// Original Implementation
					intersectlist = world.spawnRay(params.r);
				}

				int depth = 0;
				Color radiance = ThreadCamera::trace(world, params.r, Color(0, 0, 0), intersectlist, &depth);

				// divide by depth to normalize the reflected rays
				pixelArray[params.i][params.j] = radiance / (float)((depth + 1) * world.lightList.size());
			});

			pxX += pXw;
		}
		pxY -= pXh;
		pxX = -1 * this->filmPlaneWidth / 2;
	}

	// Tone Reproduction
	this->TRop->reproduceTone(pixelArray);

	generateImage(pixelArray, OUTPUT_FILENAME);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	print("\nEXECUTION TIME: ");
	std::string s = std::to_string(duration.count()) + " milliseconds\n\n";
	print(s);
}

Color ThreadCamera::trace(World world, Ray r, Color radiance, std::vector<Object::intersectResult> intersectlist, int * depth)
{
	Object::intersectResult interRes;

	// Find closest intersecting point
	if (intersectlist.empty())
	{
		// Background Color
		return world.background;
	}
	else
	{
		if (intersectlist.size() == 1)
		{
			interRes = intersectlist[0];
		}
		else
		{
			int omegaMinIndex = 0;
			for (int index = 0; index < intersectlist.size(); index++)
			{
				if (intersectlist[index].omega < omegaMinIndex)
				{
					omegaMinIndex = index;
				}
			}

			interRes = intersectlist[omegaMinIndex];
		}

		// Spawn shadow rays from P to all Light Sources
		std::vector<Ray> shadowRays;
		for (LightSource* lightSource : world.lightList)
		{
			Vector3f shadowDir = (lightSource->position.vector() - interRes.intersectPoint.vector()).normalized();
			shadowRays.push_back(Ray(interRes.intersectPoint, shadowDir));
		}

		std::vector<Vector3f> directLightVectors;
		std::vector<LightSource*> directLights;
		for (int index = 0; index < shadowRays.size(); index++)
		{
			// Check to see if shadow ray makes it to light without intersection
			std::vector<Object::intersectResult> shadowintersectlist;
			if (this->spatialFlag == 1)
			{
				shadowintersectlist = this->treeBuilder.rayThroughTree(this->KDTree, shadowRays[index]);
			}
			else
			{
				shadowintersectlist = world.spawnRay(shadowRays[index]);
			}

			if (shadowintersectlist.empty())
			{
				directLightVectors.push_back((shadowRays[index].direction));
				directLights.push_back(world.lightList[index]);
			}
			else
			{
				// check if all intersections of shadow ray hit transmissive objects
				if (std::all_of(shadowintersectlist.begin(), shadowintersectlist.end(),
					[](Object::intersectResult& shadowrayintersect) {return shadowrayintersect.mat->kt != 0; }))
				{
					Color shadowraycolor = Color(world.lightList[index]->color.r, world.lightList[index]->color.g, world.lightList[index]->color.b);
					for (Object::intersectResult& shadowrayintersect : shadowintersectlist)
					{
						shadowraycolor = shadowraycolor * shadowrayintersect.mat->kt;
					}
					directLightVectors.push_back((shadowRays[index].direction));
					directLights.push_back(new LightSource(world.lightList[index]->position, shadowraycolor));
				}
			}
		}

		// Create IntersectData
		IntersectData interData = IntersectData(interRes.intersectPoint, interRes.normal, directLightVectors,
			r.direction, directLights, world.background, world.indexRefract,
			interRes.mat->indexRefract);
		// Use illumination model for local illumination
		radiance = interRes.mat->illuminate(interData);

		if (*depth < MAX_RECUR)
		{
			// Reflective
			if (interRes.mat->kr > 0)
			{
				for (const Ray& reflectionray : interData.R)
				{
					// Calculate Intersections with world objects
					std::vector<Object::intersectResult> intersectlistreflection;
					std::vector<Object::intersectResult> temp;
					if (this->spatialFlag == 1)
					{
						temp = this->treeBuilder.rayThroughTree(this->KDTree, reflectionray);
					}
					else
					{
						temp = world.spawnRay(reflectionray);
					}

					if (temp.empty())
					{
						intersectlistreflection = temp;
					}
					else
					{
						for (const Object::intersectResult& reflectionrayintersect : temp)
						{
							if (reflectionrayintersect.mat->kt == 0)
							{
								intersectlistreflection.push_back(reflectionrayintersect);
							}
						}
					}

					(*depth)++;
					// Overloaded operator '+' to add Colors as r+r,g+g,b+b
					// Overloaded operator '*' to multiply Color * scalar (--order matters--)
					radiance =
						radiance +
						trace(world, reflectionray, radiance, intersectlistreflection, depth) * interRes.mat->kr;
				}
			}

			// Transmissive
			if (interRes.mat->kt > 0)
			{

				std::vector<Object::intersectResult> intersectlisttransmission;
				if (this->spatialFlag == 1)
				{
					intersectlisttransmission = this->treeBuilder.rayThroughTree(this->KDTree, interData.T);
				}
				else
				{
					intersectlisttransmission = world.spawnRay(interData.T);
				}
				(*depth)++;

				radiance = radiance +
					trace(world, interData.T, radiance, intersectlisttransmission, depth) * interRes.mat->kt;

			}
		}
		return radiance;
	}
}

void ThreadCamera::generateImage(std::vector<std::vector<Color>> pixelArray, const char *filename)
{
	unsigned width = this->imageWidthPx, height = this->imageHeightPx;
	std::vector<unsigned char> image;
	image.resize(width * height * 4);
	for (unsigned y = 0; y < height; y++)
		for (unsigned x = 0; x < width; x++)
		{
			image[4 * width * y + 4 * x + 0] = (unsigned char)pixelArray[y][x].r;
			image[4 * width * y + 4 * x + 1] = (unsigned char)pixelArray[y][x].g;
			image[4 * width * y + 4 * x + 2] = (unsigned char)pixelArray[y][x].b;
			image[4 * width * y + 4 * x + 3] = 255;
		}
	lodepng::encode(filename, image, width, height);
}

void ThreadCamera::setImageDim(int w, int h)
{
	this->imageHeightPx = h;
	this->imageWidthPx = w;
}

void ThreadCamera::setFilmPlaneDim(int fov, float aspect)
{
	float fovRad = fov * (PI / 180.0f);
	float w = 2 * this->focalLength * tan(fovRad / 2);
	float h = w / aspect;

	this->filmPlaneHeight = h;
	this->filmPlaneWidth = w;
}

void ThreadCamera::setFocalLength(float f)
{
	this->focalLength = f;
}

void ThreadCamera::setSpatialDataStructure(int flag)
{
	this->spatialFlag = flag;
}

std::string ThreadCamera::toString()
{
	std::string result = "ThreadCamera\n Postion\n   " + this->position.toString() + "\n";
	std::string x = std::to_string(this->lookat[0]);
	std::string y = std::to_string(this->lookat[1]);
	std::string z = std::to_string(this->lookat[2]);
	result += "Lookat\n   [" + x + " " + y + " " + z + "]\n";
	x = std::to_string(this->up[0]);
	y = std::to_string(this->up[1]);
	z = std::to_string(this->up[2]);
	result += "Up\n   [" + x + " " + y + " " + z + "]\n";
	std::stringstream ss;
	ss << viewTransform;
	result += "View Transform\n   " + ss.str() + "\n";
	result += "Focal Length\n   " + std::to_string(focalLength) + "\n";
	result += "Image Dimenstions\n   HeightPx: " + std::to_string(imageHeightPx) + " WidthPx: " +
		std::to_string(imageWidthPx) + "\n";
	result += "Film Plane Dimenstions\n   HeightPx: " + std::to_string(filmPlaneHeight) + " WidthPx: " +
		std::to_string(filmPlaneWidth) + "\n";
	return result;
}

ThreadCamera::~ThreadCamera()
{
}
