cmake_minimum_required(VERSION 3.10)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-O3")

add_executable(RayTracing main.cpp Object.hpp Vector.hpp Sphere.hpp global.cpp global.hpp Triangle.hpp Scene.cpp
        Scene.hpp Light.hpp BVH.cpp BVH.hpp Bounds3.hpp Ray.hpp Material.hpp Intersection.hpp
        Renderer.cpp Renderer.hpp)
#target_compile_options(RayTracing PUBLIC -Wall -Wextra -pedantic -Wshadow -Wreturn-type -fsanitize=undefined)
#target_compile_features(RayTracing PUBLIC cxx_std_17)
#target_link_libraries(RayTracing PUBLIC -fsanitize=undefined)