# RayTracer

This was a semester long project for the Global Illumination (CSCI-711) course at RIT. The objective was to develop a 
RayTracer in C++11 from scratch in order to render Whitted's famous inter-reflecting spheres.

[![alt text](https://blogs.nvidia.com/wp-content/uploads/2018/07/sphckbd_35mm.jpg)](https://blogs.nvidia.com/blog/2018/08/01/ray-tracing-global-illumination-turner-whitted/)

The project was divided in 7 Checkpoints:

1. Setting the Scene
2. Raytracing Framework
3. Basic Shading
4. Procedural Shading
5. Reflection
6. Refraction
7. Tone Reproduction

A KD-Tree was implemented as the acceleration structure of the RayTracer. 

The output of each Checkpoint is Whitted's spheres and can be found in their corresponding directory inside the `/checkpoints` directory.

A sneak peak of the results of each checkpoint is shown below:


## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. 

### Prerequisites

Download the following software to be able to run the project.

#### Windows
- [Visual Studio 17](https://visualstudio.microsoft.com/)
    - [Nuget](https://www.nuget.org/) - Download with Visual Studio Installer 

#### macOS
- [CLion](https://www.jetbrains.com/clion/)
- [HomeBrew](https://brew.sh/)

### Installing

Download or clone the code
```
git clone https://github.com/cozzarin787/Raytracer.git
```

#### Windows
1. Open Visual Studio 2017
2. Click on `Open Project/Solution`
3. Select the `RayTracer.sln` file inside the `/RayTracer` directory 
4. Right click on the solution
5. Click on `Manage Nuget Packages...`
6. Install `Eigen` & `LodePNG`

The project should be ready to run at this point. 
Running the program should render Whitted's spheres inside the `/RayTracer/RayTracer` directory.

#### macOS
1. Download [lodepng.cpp]() & [lodepng.h]() into the project directory with all the other C++ files
2. Install `Eigen`
    ```
    brew install eigen
    ```
3. Open CLion
4. Select Open and choose the project directory `/RayTracer`

The `CMakeLists.txt` file should load the configuration. In case it doesn't just select it.
The project should be ready to run at this point. 
Running the program should render Whitted's spheres inside the `/RayTracer/cmake-build-debug` directory.

## Versioning

For the versions available, see the [tags on this repository](https://github.com/cozzarin787/Raytracer/tags). 

## Authors

* **Jacob Cozzarin** <[cozzarin787](https://github.com/cozzarin787)>
* **Pablo Ordorica** <[pablordoricaw](https://github.com/pablordoricaw)>


## Acknowledgments

* **Prof. Joe Geigel** <[jmg](https://www.cs.rit.edu/~jmg/#welcome)> - For teaching the course and all his help throughout the project

