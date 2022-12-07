#include "Renderer.hpp"
#include "Scene.hpp"
#include "Triangle.hpp"
#include "Sphere.hpp"
#include "Vector.hpp"
#include "global.hpp"
#include <chrono>

// In the main function of the program, we create the scene (create objects and
// lights) as well as set the options for the render (image width and height,
// maximum recursion depth, field-of-view, etc.). We then call the render
// function().
int main(int argc, char** argv)
{

    // Change the definition here to change resolution
    Scene scene(100, 100);
    // Scene scene(100, 100);

    Material* red = new Material(DIFFUSE, Vector3f(0.0f));
    red->Kd = Vector3f(0.63f, 0.065f, 0.05f);
    Material* green = new Material(DIFFUSE, Vector3f(0.0f));
    green->Kd = Vector3f(0.14f, 0.45f, 0.091f);
    Material* white = new Material(DIFFUSE, Vector3f(0.0f));
    white->Kd = Vector3f(0.725f, 0.71f, 0.68f);
    Material* light = new Material(DIFFUSE, (8.0f * Vector3f(0.747f+0.058f, 0.747f+0.258f, 0.747f) + 15.6f * Vector3f(0.740f+0.287f,0.740f+0.160f,0.740f) + 18.4f *Vector3f(0.737f+0.642f,0.737f+0.159f,0.737f)));
    light->Kd = Vector3f(0.65f);

    Material* iron = new Material(MICROFACET, Vector3f(0.0f));
    iron->rho = Vector3f(0.725f, 0.71f, 0.68f);
    iron->F0 = Vector3f(0.77f,0.78f,0.78f);
    iron->alpha = 0.1f;

    Material* gold = new Material(MICROFACET, Vector3f(0.0f));
    gold->rho = Vector3f(0.725f, 0.71f, 0.68f);
    gold->F0 = Vector3f(1.0f,0.86f,0.57f);
    gold->alpha = 0.1f;

    Material* plastic = new Material(MICROFACET, Vector3f(0.0f));
    plastic->rho = Vector3f(0.725f, 0.71f, 0.68f);
    plastic->F0 = Vector3f(0.24f,0.24f,0.24f);
    plastic->alpha = 0.1f;

    Material* redGlass = new Material(MICROFACET, Vector3f(0.0f));
    redGlass->rho = Vector3f(0.63f, 0.065f, 0.05f);
    redGlass->F0 = Vector3f(0.31f,0.31f,0.31f);
    redGlass->alpha = 0.1f;

    Material* water = new Material(MICROFACET, Vector3f(0.0f));
    water->rho = Vector3f(0.725f, 0.71f, 0.68f);
    water->F0 = Vector3f(0.15f,0.15f,0.15f);
    water->alpha = 0.1f;

    Mesh floor("../models/cornellbox/floor.obj", white);
    // Mesh shortbox("../models/cornellbox/shortbox.obj", white);
    // Mesh tallbox("../models/cornellbox/tallbox.obj", white);
    Mesh left("../models/cornellbox/left.obj", red);
    Mesh right("../models/cornellbox/right.obj", green);
    Mesh light_("../models/cornellbox/light.obj", light);

    Mesh bunny("../models/bunny/bunny2.obj", gold);
    // Mesh shortbox("../models/cornellbox/shortbox.obj", plastic);
    // Mesh tallbox("../models/cornellbox/tallbox.obj", gold);

    scene.Add(&floor);
    // scene.Add(&shortbox);
    // scene.Add(&tallbox);
    scene.Add(&left);
    scene.Add(&right);
    scene.Add(&light_);

    scene.Add(&bunny);

    scene.buildBVH();

    Renderer r;

    auto start = std::chrono::system_clock::now();
    r.Render(scene);
    auto stop = std::chrono::system_clock::now();

    std::cout << "Render complete: \n";
    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::hours>(stop - start).count() << " hours\n";
    std::cout << "          : " << std::chrono::duration_cast<std::chrono::minutes>(stop - start).count() << " minutes\n";
    std::cout << "          : " << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " seconds\n";

    return 0;
}