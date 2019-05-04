#include "Camera.h"
#include "IntersectData.h"
#include "Checkerboard.h"
#include "TRReinhard.h"
#include <lodepng.h>
#include <iostream>

#define PI 3.141592654f
#define print(x) std::cout << x << std::endl;
#define MAX_DEPTH 2
#define OUTPUT_FILENAME "test.png"

Camera::Camera(Point p, RowVector3f lookat, RowVector3f up) {
    this->position = p;
    this->lookat = lookat;
    this->up = up;
    // default value
    this->focalLength = 1;

    // construct view transform
    RowVector3f n = lookat.normalized();
    RowVector3f u = (up.cross(n)).normalized();
    RowVector3f v = n.cross(u);

    this->viewTransform.row(0) << u[0], u[1], u[2], -1 * (p.vector().dot(u));
    this->viewTransform.row(1) << v[0], v[1], v[2], -1 * (p.vector().dot(v));
    this->viewTransform.row(2) << n[0], n[1], n[2], -1 * (p.vector().dot(n));
    this->viewTransform.row(3) << 0, 0, 0, 1;

	// Set default tone reproduction operator
	TRReinhard trop = TRReinhard(300);
	this->TRop = &trop;
}

Camera::Camera(Point p, RowVector3f lookat, RowVector3f up, TROperator* trop)
{
	this->position = p;
	this->lookat = lookat;
	this->up = up;
	// default value
	this->focalLength = 1;

	// construct view transform
	RowVector3f n = lookat.normalized();
	RowVector3f u = (up.cross(n)).normalized();
	RowVector3f v = n.cross(u);

	this->viewTransform.row(0) << u[0], u[1], u[2], -1 * (p.vector().dot(u));
	this->viewTransform.row(1) << v[0], v[1], v[2], -1 * (p.vector().dot(v));
	this->viewTransform.row(2) << n[0], n[1], n[2], -1 * (p.vector().dot(n));
	this->viewTransform.row(3) << 0, 0, 0, 1;

	this->TRop = trop;
}

void Camera::render(World world) {
    // transform world into camera coordinates
    world.transformAllObjects(this->viewTransform);

    // transform lightsources into camera coordinates
    world.transformAllLights(this->viewTransform);

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
    for (int i = 0; i < this->imageHeightPx; i++) {
        for (int j = 0; j < this->imageWidthPx; j++) {
            // Spawn Ray through the middle of a pixel
            Point pxpos = Point(pxX + pXw, pxY - pXh, this->focalLength);
            Point cameraOrigin = Point(0, 0, 0);
            RowVector3f rayvec = (pxpos.vector() - cameraOrigin.vector()).normalized();
            Ray r = Ray(cameraOrigin, rayvec);

            // Calculate Intersections with world objects
            std::vector<Object::intersectResult> intersectlist = world.spawnRay(r);

            r.direction *= -1;

            int depth = 0;
            Color radiance = trace(world, r, Color(0, 0, 0), intersectlist, &depth);

            // divide by depth to normalize the reflected rays
            pixelArray[i][j] = radiance/ (depth + 1);

            pxX += pXw;
        }
        pxY -= pXh;
        pxX = -1 * this->filmPlaneWidth / 2;
    }

    // Tone Reproduction
	this->TRop->reproduceTone(pixelArray);

    generateImage(pixelArray, OUTPUT_FILENAME);
}

Color Camera::trace(World world, Ray r, Color radiance, std::vector<Object::intersectResult> intersectlist, int * depth) {
//    Color radiance;

    Object::intersectResult interRes;

    // Find closest intersecting point
    if (intersectlist.empty()) {
        // Background Color
        return world.background;
    } else {
        if (intersectlist.size() == 1) {
            interRes = intersectlist[0];
        } else {
            int omegaMinIndex = 0;
            for (int index = 0; index < intersectlist.size(); index++) {
                if (intersectlist[index].omega < omegaMinIndex) {
                    omegaMinIndex = index;
                }
            }

            interRes = intersectlist[omegaMinIndex];
        }

        // Spawn shadow rays from P to all Light Sources
        std::vector<Ray> shadowRays;
        for (int index = 0; index < world.lightList.size(); index++) {
            RowVector3f shadowDir = (world.lightList[index].position.vector() -
                                     interRes.intersectPoint.vector()).normalized();
            shadowRays.push_back(Ray(interRes.intersectPoint, shadowDir));
        }

        std::vector<RowVector3f> directLightVectors;
        std::vector<LightSource> directLights;
        for (int index = 0; index < shadowRays.size(); index++) {
            // Check to see if shadow ray makes it to light without intersection
            if (world.spawnRay(shadowRays[index]).empty()) {
                directLightVectors.push_back((shadowRays[index].direction));
                directLights.push_back(world.lightList[index]);
            }
        }

        // Create IntersectData
        IntersectData interData = IntersectData(interRes.intersectPoint, interRes.normal, directLightVectors,
                                                r.direction, directLights, world.background);

        // Use illumination model for local illumination
        radiance = interRes.mat->illuminate(interData);

        if (*depth < MAX_DEPTH) {
            // Reflective
            if (interRes.mat->kr > 0) {
                for (int i = 0; i < interData.R.size(); i++) {
                    // Calculate Intersections with world objects
                    std::vector<Object::intersectResult> intersectlistreflection = world.spawnRay(interData.R[i]);
                    (*depth)++;
                    // Overloaded operator '+' to add Colors as r+r,g+g,b+b
                    // Overloaded operator '*' to multiply Color * scalar (--order matters--)
                    radiance =
                            radiance +
                            trace(world, interData.R[i], radiance, intersectlistreflection, depth ) * interRes.mat->kr;
                }
            }

            // Transmisive
            if (interRes.mat->kt > 0) {
                // TODO
            }
        }
        return radiance;
    }
}


void Camera::generateImage(std::vector<std::vector<Color>> pixelArray, const char *filename) {
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

void Camera::setImageDim(int w, int h) {
    this->imageHeightPx = h;
    this->imageWidthPx = w;
}

void Camera::setFilmPlaneDim(int fov, float aspect) {
    float fovRad = fov * (PI / 180.0f);
    float w = 2 * this->focalLength * tan(fovRad / 2);
    float h = w / aspect;

    this->filmPlaneHeight = h;
    this->filmPlaneWidth = w;
}

void Camera::setFocalLength(float f) {
    this->focalLength = f;
}

std::string Camera::toString() {
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
    result += "Image Dimenstions\n   HeightPx: " + std::to_string(imageHeightPx) + " WidthPx: " +
              std::to_string(imageWidthPx) + "\n";
    result += "Film Plane Dimenstions\n   HeightPx: " + std::to_string(filmPlaneHeight) + " WidthPx: " +
              std::to_string(filmPlaneWidth) + "\n";
    return result;
}

Camera::~Camera() {
}

// BUILD K-D TREE
/*
	N = getNode(voxel, List of primatives)
		if (List of Primatives size == 1)
			return leafNode(List of primatives)
		P = get partition plane
		L_1 = objects "above P"
		L_2 = objects "below P"
		top = getNode(topVoxel, L_1)
		bottom = getNode(bottomVoxel, L_2)
*/
// Spawn Ray
/*
	intersect(KDTree Node N, Ray R)
		if (N is a leaf):
			go though elements in N and return closest.
		else:
			a = where R enters the voxel (changing coordinate)
			b = where R leaves the voxel (changing coordinate)
			S = seperating plane
			case 1: Only crosses top voxel (a and b above P)
				intersect(N.top, R)
			case 2: Only crosses bottom voxel (a and b below P)
				intersect(N.bottom, R)
			case 3: Starts top, goes to bottom (P between a and b, where a > b)
				intersect(N.top, R)
				if (no intersection)
					intersect(N.bottom, R)
			case 4: Starts bottom, goes to top (P between a and b, where b > a)
				intersect(N.bottom, R)
				if (no intersection)
					intersect(N.top, R)
*/
