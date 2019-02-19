#include "Camera.h"
#include "IntersectData.h"
#include <lodepng.h>
#include <iostream>
#define PI 3.141592654
#define print(x) std::cout << x << std::endl;

Camera::Camera(Point p, RowVector3f lookat, RowVector3f up)
{
	this->position = p;
	this->lookat = lookat;
	this->up = up;
	// default value
	this->focalLength = 1;

	// construct view transform
	RowVector3f n = (p.vector() - lookat).normalized();
	RowVector3f u = (up.cross(n)).normalized();
	RowVector3f v = u.cross(n);

	this->viewTransform.row(0) << u[0], u[1], u[2], -1 * (p.vector().dot(u));
	this->viewTransform.row(1) << v[0], v[1], v[2], -1 * (p.vector().dot(v));
	this->viewTransform.row(2) << n[0], n[1], n[2], -1 * (p.vector().dot(n));
	this->viewTransform.row(3) << 0, 0, 0, 1;
}

void Camera::render(World world)
{
	// transform world into camera coordinates
	world.transformAllObjects(this->viewTransform);

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
	for (int i = 0; i < this->imageHeightPx; i++)
	{
		for (int j = 0; j < this->imageWidthPx; j++)
		{
			// Spawn Ray through the middle of a pixel
			Point pxpos = Point(pxX + pXw, pxY - pXh, this->focalLength);
			Point cameraOrigin = Point(0, 0, 0);
			RowVector3f rayvec = (pxpos.vector() - cameraOrigin.vector()).normalized();
			Ray r = Ray(cameraOrigin, rayvec);

			// Calculate Intersections with world objects
			std::vector<Object::intersectResult> intersectlist = world.spawnRay(r);

			RowVector3f radiance;
			Object::intersectResult interRes;

			// Find closest intersecting point
			if (intersectlist.empty())
			{
				// Background Color
				radiance = RowVector3f(30, 144, 255);
			}
			else if (intersectlist.size() == 1)
			{
				interRes = intersectlist[0];
			}
			else
			{
				int omegaMin = 0;
				for (int i = 0; i < intersectlist.size(); i++)
				{
					if (intersectlist[i].omega < omegaMin)
					{
						omegaMin = i;
					}
				}

				interRes = intersectlist[omegaMin];
			}

			// Spawn shadow ray from P to Light Source
			RowVector3f shadowDir = (world.lightList[0].position.vector() - interRes.intersectPoint.vector()).normalized();
			Ray shadowRay = Ray(interRes.intersectPoint, shadowDir);

			// Check to see if shadow ray makes it to light without intersection
			if (world.spawnRay(shadowRay).empty())
			{
				// Create IntersectData
				IntersectData interData = IntersectData(interRes.intersectPoint, interRes.normal, -1 * shadowDir, -1 * r.direction, world.lightList);

				// Use illumination model
				radiance = interRes.mat->illuminate(interData);
			}
			else
			{
				// TODO
				// Calculate ambient component
			}

			pixelArray[i][j] = Color(radiance[0], radiance[1], radiance[2]);


			pxX += pXw;
		}
		pxY -= pXh;
		pxX = -1 * this->filmPlaneWidth / 2;
	}

	//generate some image
	const char* filename = "test.png";

	unsigned width = this->imageWidthPx, height = this->imageHeightPx;
	std::vector<unsigned char> image;
	image.resize(width * height * 4);
	for (unsigned y = 0; y < height; y++)
		for (unsigned x = 0; x < width; x++) {
			image[4 * width * y + 4 * x + 0] = pixelArray[y][x].r;
			image[4 * width * y + 4 * x + 1] = pixelArray[y][x].g;
			image[4 * width * y + 4 * x + 2] = pixelArray[y][x].b;
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
	float fovRad = fov * (PI / 180);
	float w = 2 * this->focalLength * tan(fovRad / 2);
	float h = w / aspect;

	this->filmPlaneHeight = h;
	this->filmPlaneWidth = w;
}

void Camera::setFocalLength(float f)
{
	this->focalLength = f;
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
	result += "View Transform\n   " + ss.str() + "\n";
	result += "Focal Length\n   " + std::to_string(focalLength) + "\n";
	result += "Image Dimenstions\n   HeightPx: " + std::to_string(imageHeightPx) + " WidthPx: " + std::to_string(imageWidthPx) + "\n";
	result += "Film Plane Dimenstions\n   HeightPx: " + std::to_string(filmPlaneHeight) + " WidthPx: " + std::to_string(filmPlaneWidth) + "\n";
	return result;
}

Camera::~Camera()
{
}
