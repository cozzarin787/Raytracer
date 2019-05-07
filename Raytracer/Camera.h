#pragma once
#include "Point.h"
#include "Ray.h"
#include "World.h"
#include "TROperator.h"
#include "KdInterior.h"
#include "KdLeaf.h"

using Eigen::Matrix4f;

class Camera
{
private:
	float focalLength;
	int imageHeightPx;
	int imageWidthPx;
	float filmPlaneHeight;
	float filmPlaneWidth;
	TROperator* TRop;
	int spatialFlag;

public:
	Point position;
	Vector3f lookat;
	Vector3f up;
	Matrix4f viewTransform;

	Camera(Point p, Vector3f lookat, Vector3f up);
	Camera(Point p, Vector3f lookat, Vector3f up, TROperator* trop);

	void render(World world);
	Color trace(World world, Ray r, Color radiance, std::vector<Object::intersectResult> intersectlist, int * depth);
	void generateImage(std::vector<std::vector<Color>> pixelArray, const char * filename);
	void setImageDim(int w, int h);
	void setFilmPlaneDim(int fov, float aspect);
	void setFocalLength(float f);
	void setSpatialDataStructure(int flag);

	std::string toString();

	~Camera();
};

