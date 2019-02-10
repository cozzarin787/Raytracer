#include "Camera.h"
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
	this->focalLength = 50;

	// construct view transform
	RowVector3f n = (p.vector() - lookat).normalized();
	RowVector3f u = (up.cross(n)).normalized();
	RowVector3f v = n.cross(u);

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
	Color** pixelArray = new Color*[this->imageHeightPx];
	for (int i = 0; i < this->imageHeightPx; i++)
		pixelArray[i] = new Color[this->imageWidthPx];

	float pXh = this->filmPlaneHeight / this->imageHeightPx;
	float pXw = this->filmPlaneWidth / this->imageWidthPx;

	float pxY = this->filmPlaneWidth / 2;
	float pxX = -1* this->filmPlaneHeight / 2;

	// Create the array of pixels representing the rendered image of the world
	int count = 0;
	for (int i = 0; i < this->imageHeightPx; i++)
	{
		for (int j = 0; j < this->imageWidthPx; j++)
		{
			count++;
			// Spawn Ray at pixel position
			Point pxpos = Point(pxX + pXw, pxY - pXh, this->focalLength);
			RowVector3f rayvec = (pxpos.vector() - this->position.vector()).normalized();
			Ray r = Ray(this->position, rayvec);

			// Calculate Intersections with world objects
			std::vector<Object::intersectResult> intersectlist = world.spawnRay(r);

			// calculate color
			if (intersectlist.empty())
			{
				// Background Color
				pixelArray[i][j] = Color(30, 144, 255, 255);
			}
			else if (intersectlist.size() == 1)
			{
				pixelArray[i][j] = intersectlist[0].mat.color;
			}
			else
			{
				float omegaMin = intersectlist[0].omega;
				Color minColor = intersectlist[0].mat.color;
				for (Object::intersectResult ir : intersectlist)
				{
					if (ir.omega < omegaMin)
					{
						omegaMin = ir.omega;
						minColor = ir.mat.color;
					}
				}
				pixelArray[i][j] = minColor;
			}
			pxX += pXw;
		}
		pxY -= pXw;
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
			image[4 * width * y + 4 * x + 3] = pixelArray[y][x].a;
			//print(pixelArray[y][x].r)
		}
	lodepng::encode(filename, image, width, height);

	// Create image from pixelArray
	//std::vector<std::uint8_t> pngBuffer(this->imageHeightPx*this->imageWidthPx * 4);

	//for (int32_t i = 0; i < this->imageHeightPx; i++)
	//{
	//	for (int32_t j = 0; j < this->imageWidthPx; j++)
	//	{
	//		std::size_t index = i * (this->imageWidthPx * 4) + 4 * j;
	//		pngBuffer[index + 0] = pixelArray[i][j].r;
	//		pngBuffer[index + 1] = pixelArray[i][j].g;
	//		pngBuffer[index + 2] = pixelArray[i][j].b;
	//		pngBuffer[index + 3] = pixelArray[i][j].a;
	//		/*std::string str1 = std::to_string(pngBuffer[index + 0]);
	//		std::string str2 = std::to_string(pngBuffer[index + 1]);
	//		std::string str3 = std::to_string(pngBuffer[index + 2]);
	//		std::string str4 = std::to_string(pngBuffer[index + 3]);
	//		print(pixelArray[i][j].toString());*/
	//	}
	//}
	
	// Free pixelArray pointer
	/*for (uint8_t i = 0; i < imageHeightPx; i++)
	{
		delete pixelArray[i];
	}

	delete pixelArray;*/

	/*std::vector<uint8_t> imageBuffer;
	lodepng::encode(imageBuffer, pngBuffer, this->imageWidthPx, this->imageHeightPx);
	lodepng::save_file(imageBuffer, "rendered.png");*/
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
