#include "Camera.h"
#include "IntersectData.h"
#include "Checkerboard.h"
#include "KdTreeBuilder.h"
#include <lodepng.h>
#include <iostream>
#include <chrono>
#define PI 3.141592654f
#define print(x) std::cout << x << std::endl;

using namespace std::chrono;

Camera::Camera(Point p, Vector3f lookat, Vector3f temp)
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
}

void Camera::render(World world)
{
	// transform world into camera coordinates
	world.transformAllObjects(this->viewTransform);

	// transform lightsources into camera coordinates
	world.transformAllLights(this->viewTransform);

	// calculate the bounding box of the world
	world.calcWorldVoxel();

	// BUILD K-D TREE
	KdTreeBuilder treeBuilder;
	KdNode* KDTree = &KdLeaf();
	if (spatialFlag == 1)
	{
		auto start = high_resolution_clock::now();
		treeBuilder = KdTreeBuilder();
		KDTree = treeBuilder.getNode(world.totalBound, world.voxelObjectList, 0);
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
	float pXw = this->filmPlaneWidth / (float) this->imageWidthPx;

	float pxY = this->filmPlaneHeight / 2;
	float pxX = -1 * this->filmPlaneWidth / 2;

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

			std::vector<Object::intersectResult> intersectlist;

			// Calculate Intersections with world objects by sending ray through KDTree
			if (spatialFlag == 1)
			{
				intersectlist = treeBuilder.rayThroughTree(KDTree, r);
			}
			else
			{
				// Original Implementation
				intersectlist = world.spawnRay(r);
			}

			Color radiance;
			Object::intersectResult interRes;

			// Find closest intersecting point
			if (intersectlist.empty())
			{
				// Background Color
				radiance = world.background;
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
				for (int index = 0; index < world.lightList.size(); index++)
				{
					Vector3f shadowDir = (world.lightList[index]->position.vector() - interRes.intersectPoint.vector()).normalized();
					shadowRays.push_back(Ray(interRes.intersectPoint, shadowDir));
				}

				std::vector<Vector3f> directLightVectors;
				std::vector<LightSource*> directLights;
				for (int index = 0; index < shadowRays.size(); index++)
				{
					// Check to see if shadow ray makes it to light without intersection
					if (spatialFlag == 1)
					{
						if ((treeBuilder.rayThroughTree(KDTree, shadowRays[index])).empty())
						{
							directLightVectors.push_back((shadowRays[index].direction));
							directLights.push_back(world.lightList[index]);
						}
					}
					else
					{
						if (world.spawnRay(shadowRays[index]).empty())
						{
							directLightVectors.push_back((shadowRays[index].direction));
							directLights.push_back(world.lightList[index]);
						}
					}
				}
				
				// Create IntersectData
				IntersectData interData = IntersectData(interRes.intersectPoint, interRes.normal, directLightVectors, -1 * r.direction, directLights, world.background);

				// Use illumination model
				radiance = interRes.mat->illuminate(interData);
			}

			pixelArray[i][j] = radiance;

			pxX += pXw;
		}
		pxY -= pXh;
		pxX = -1 * this->filmPlaneWidth / 2;
	}

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	print("\nEXECUTION TIME: ");
	std::string s = std::to_string(duration.count()) + " milliseconds\n\n";
	print(s);

	// Tone Reproduction
	// Find max radiance value
	float maxRadiance = pixelArray[0][0].r;
	for (int i = 0; i < this->imageHeightPx; i++)
	{
		for (int j = 0; j < this->imageWidthPx; j++)
		{
			if (pixelArray[i][j].r > maxRadiance)
			{
				maxRadiance = pixelArray[i][j].r;
			}
			else if (pixelArray[i][j].g > maxRadiance)
			{
				maxRadiance = pixelArray[i][j].g;
			}
			else if (pixelArray[i][j].b > maxRadiance)
			{
				maxRadiance = pixelArray[i][j].b;
			}
		}
	}

	// Interpolate each radiance value to a 255 scale to create png
	for (int i = 0; i < this->imageHeightPx; i++)
	{
		for (int j = 0; j < this->imageWidthPx; j++)
		{
			pixelArray[i][j].r = 255 * (pixelArray[i][j].r / maxRadiance);
			pixelArray[i][j].g = 255 * (pixelArray[i][j].g / maxRadiance);
			pixelArray[i][j].b = 255 * (pixelArray[i][j].b / maxRadiance);
		}
	}

	//generate some image
	const char* filename = "test.png";

	unsigned width = this->imageWidthPx, height = this->imageHeightPx;
	std::vector<unsigned char> image;
	image.resize(width * height * 4);
	for (unsigned y = 0; y < height; y++)
		for (unsigned x = 0; x < width; x++) {
			image[4 * width * y + 4 * x + 0] = (unsigned char) pixelArray[y][x].r;
			image[4 * width * y + 4 * x + 1] = (unsigned char) pixelArray[y][x].g;
			image[4 * width * y + 4 * x + 2] = (unsigned char) pixelArray[y][x].b;
			image[4 * width * y + 4 * x + 3] = 255;
		}
	lodepng::encode(filename, image, width, height);
}

void Camera::setImageDim(int w, int h)
{
	this->imageHeightPx = h;
	this->imageWidthPx = w;
}

void Camera::setFilmPlaneDim(int fov, float aspect)
{
	float fovRad = fov * (PI / 180.0f);
	float w = 2 * this->focalLength * tan(fovRad / 2);
	float h = w / aspect;

	this->filmPlaneHeight = h;
	this->filmPlaneWidth = w;
}

void Camera::setFocalLength(float f)
{
	this->focalLength = f;
}

void Camera::setSpatialDataStructure(int flag)
{
	this->spatialFlag = flag;
}

std::string Camera::toString()
{
	std::string result = "Camera\n Postion\n   " + this->position.toString() + "\n";
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
	result += "View Transform\n" + ss.str() + "\n";
	result += "Focal Length\n   " + std::to_string(focalLength) + "\n";
	result += "Image Dimenstions\n   HeightPx: " + std::to_string(imageHeightPx) + " WidthPx: " + std::to_string(imageWidthPx) + "\n";
	result += "Film Plane Dimenstions\n   HeightPx: " + std::to_string(filmPlaneHeight) + " WidthPx: " + std::to_string(filmPlaneWidth) + "\n";
	return result;
}

Camera::~Camera()
{
}
