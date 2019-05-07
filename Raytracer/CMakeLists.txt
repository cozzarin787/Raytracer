cmake_minimum_required(VERSION 3.13)
project(Raytracer)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
find_package (Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(${EIGEN3_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Raytracer
        Camera.cpp
        Camera.h
        Checkerboard.cpp
        Checkerboard.h
        Color.cpp
        Color.h
        IntersectData.cpp
        IntersectData.h
        LightSource.cpp
        LightSource.h
        Material.cpp
        Material.h
        Object.cpp
        Object.h
        PhongBlinn.cpp
        PhongBlinn.h
        Phong.cpp
        Phong.h
        Plane.cpp
        Plane.h
        Point.cpp
        Point.h
        Polygon.cpp
        Polygon.h
        Ray.cpp
        Ray.h
        Source.cpp
        Sphere.cpp
        Sphere.h
        Triangle.cpp
        Triangle.h
        TROperator.cpp
        TROperator.h
        TRReinhard.cpp
        TRReinhard.h
        TRWard.cpp
        TRWard.h
        World.cpp
        World.h
        lodepng.cpp
        lodepng.h)
