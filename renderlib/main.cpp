#include "frameBuffer.h"
#include "camera.h"
#include "shape.h"
#include "shader.h"
#include <iostream>
#include <limits>
#include <string>
#include <vector>

// Build as a separate executable, or replace your existing main.cpp.
// Requires your Lambertian and Normal shader implementations.
// Assumes shader.h exposes PointLight with public position and color fields
// and a default constructor. Adjust the light setup below if yours differs.
// Make sure the hit-point member used in shader.cpp matches HitStruct:
// the previously saved shape.h calls it point, not intersectionPoint.

void printUsage(const char* program) {
    std::cout << "Usage: " << program << " [options]\n"
              << "  --width N           Framebuffer width (default 200)\n"
              << "  --height N          Framebuffer height (default 200)\n"
              << "  --position X Y Z    Camera position (default 0 0 0)\n"
              << "  --direction X Y Z   View direction (default 0 0 -1)\n"
              << "  --focal-length F    Positive focal length (default 1.0)\n"
              << "  --cam-width W       Positive camera plane width (default 0.5)\n"
              << "  --output FILE       Output PNG (default render.png)\n"
              << "  --help              Show this help\n";
}

int main(int argc, char* argv[]) {
    int width = 200;
    int height = 200;
    vec3 position(0, 0, 0);
    vec3 viewDirection(0, 0, -1);
    double focalLength = 1.0;
    double camWidth = 0.5;
    std::string outputFile = "render.png";

    // Assume each option is valid and is followed by the required values.
    for (int i = 1; i < argc; ++i) {
        std::string option = argv[i];

        if (option == "--width") {
            width = std::stoi(argv[++i]);
        } else if (option == "--height") {
            height = std::stoi(argv[++i]);
        } else if (option == "--position") {
            double x = std::stod(argv[++i]);
            double y = std::stod(argv[++i]);
            double z = std::stod(argv[++i]);
            position = vec3(x, y, z);
        } else if (option == "--direction") {
            double x = std::stod(argv[++i]);
            double y = std::stod(argv[++i]);
            double z = std::stod(argv[++i]);
            viewDirection = vec3(x, y, z);
        } else if (option == "--focal-length") {
            focalLength = std::stod(argv[++i]);
        } else if (option == "--cam-width") {
            camWidth = std::stod(argv[++i]);
        } else if (option == "--output") {
            outputFile = argv[++i];
        } else if (option == "--help" || option == "-h") {
            printUsage(argv[0]);
            return 0;
        }
    }

    frameBuffer fb(width, height);
    vec3 white(1, 1, 1);
    vec3 red(1.0,  0.0, 0.0);
    vec3 orange(1.0,  0.5, 0.0);
    vec3 yellow(1.0,  1.0, 0.0);
    vec3 green(0.0,  1.0, 0.0);
    vec3 blue(0.0,  0.0, 1.0);
    vec3 indigo(0.29, 0.0, 0.51);
    vec3 violet(0.56, 0.0, 1.0);
    vec3 black(0.0, 0.0, 0.0);


    PerspectiveCamera p(viewDirection, position,
                        fb.getWidth(), fb.getHeight(), focalLength, camWidth);

    Sphere redSphere(vec3(0, 0.5, -6), 0.5);
    Sphere orangeSphere(vec3(-0.3, 1.5, -6), 0.5);
    Sphere whiteSphere(vec3(0,-100.5,-1), 100);
    

    Lambertian lambertian;
    Normal normalShader;
    BlinnPhong bpShader;


    struct SceneObject {
        Shape* shape;
        Shader* shader;
        vec3 color;
    };

    SceneObject objects[] = {
        {&redSphere,    &lambertian, vec3(1.0, 0.0, 0.0)},
        {&orangeSphere, &bpShader, vec3(1.0, 0.5, 0.0)},
        {&whiteSphere, &bpShader, vec3(1.0, 1.0, 1.0)}

    };

    std::vector<Shape*> shapes;

    for (const SceneObject& object : objects) {
        shapes.push_back(object.shape);
    }
    
    // A white point light above and to the right of the camera.
    PointLight light1;
    PointLight light2;
    //light1.position = vec3(-0.6, 0.1, 0);
    light1.position = vec3(0, 10, -6);
    //light2.position = vec3(-0.6, 1, 0);
    std::vector<PointLight> vecLights{light1/*,light2*/};

    for (int x = 0; x < fb.getWidth(); ++x) {
        for (int y = 0; y < fb.getHeight(); ++y) {
            ray r = p.generateRay(x, y);
            const float tmin = 1.0f;
            float tmax = std::numeric_limits<float>::infinity();
            HitStruct closestHit;
            const SceneObject* closestObject = nullptr;
            vec3 pixelColor = black;

            // intersect must lower tmax on each accepted hit, as in your
            // Sphere implementation. Save that hit before testing again.
            for (const SceneObject& object : objects) {
                HitStruct candidate;
                if (object.shape->intersect(r, tmin, tmax, candidate)) {
                    candidate.viewDir = r.getDirection();
                    closestHit = candidate;
                    closestObject = &object;
                }
            }

            // Shade only the closest surface, after testing all objects.
            if (closestObject != nullptr) {
                vec3 shading = closestObject->shader->rayColor(closestHit, vecLights, shapes);
                const vec3& base = closestObject->color;
                // Tint the Lambertian sphere red. A white tint preserves
                // all RGB components returned by the Normal shader.
                pixelColor = vec3(base.x() * shading.x(),
                                  base.y() * shading.y(),
                                  base.z() * shading.z());
            }
            fb.set(x, y, pixelColor);
        }
    }

    fb.exportPNG(outputFile);
    std::cout << "Rendered shapes to " << outputFile << '\n';
    return 0;
}