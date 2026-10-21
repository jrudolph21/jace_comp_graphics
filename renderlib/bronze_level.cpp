#include "rayTracer.h"
#include <iostream>
#include <stdexcept>
#include <string>

// ./renderlib/Debug/frameBuffer.exe --width 500 --height 500 --depth 5 --output render.png

void printUsage_lab(const char* program) {
    std::cout << "Usage: " << program << " [options]\n"
              << "  --width N           Framebuffer width (default 200)\n"
              << "  --height N          Framebuffer height (default 200)\n"
              << "  --position X Y Z    Camera position (default 0 1.8 3)\n"
              << "  --direction X Y Z   View direction (default 0 -0.22 -1)\n"
              << "  --focal-length F    Focal length (default 0.4)\n"
              << "  --cam-width W       Camera plane width (default 0.5)\n"
              << "  --depth N           Recursive depth (default 3)\n"
              << "  --input FILE        Reserved for JSON loading\n"
              << "  --output FILE       Output PNG (default render.png)\n"
              << "  --help              Show this help\n";
}

int main(int argc, char* argv[]) {
    int width = 200;
    int height = 200;
    int depth = 3;

    vec3 position(0, 1.8, 3.0);
    vec3 viewDirection(0, -0.22, -1);

    double focalLength = 0.4;
    double camWidth = 0.5;

    std::string inputFile;
    std::string outputFile = "render.png";

    try {
        for (int i = 1; i < argc; ++i) {
            std::string option = argv[i];

            // Check that the option has enough following arguments.
            auto requireArguments = [&](int count) {
                if (i + count >= argc) {
                    throw std::runtime_error(
                        "Missing argument for " + option
                    );
                }
            };

            if (option == "--width") {
                requireArguments(1);
                width = std::stoi(argv[++i]);
            } else if (option == "--height") {
                requireArguments(1);
                height = std::stoi(argv[++i]);
            } else if (option == "--depth") {
                requireArguments(1);
                depth = std::stoi(argv[++i]);
            } else if (option == "--position") {
                requireArguments(3);

                double x = std::stod(argv[++i]);
                double y = std::stod(argv[++i]);
                double z = std::stod(argv[++i]);

                position = vec3(x, y, z);
            } else if (option == "--direction") {
                requireArguments(3);

                double x = std::stod(argv[++i]);
                double y = std::stod(argv[++i]);
                double z = std::stod(argv[++i]);

                viewDirection = vec3(x, y, z);
            } else if (option == "--focal-length") {
                requireArguments(1);
                focalLength = std::stod(argv[++i]);
            } else if (option == "--cam-width") {
                requireArguments(1);
                camWidth = std::stod(argv[++i]);
            } else if (option == "--input") {
                requireArguments(1);
                inputFile = argv[++i];
            } else if (option == "--output") {
                requireArguments(1);
                outputFile = argv[++i];
            } else if (option == "--help" || option == "-h") {
                printUsage_lab(argv[0]);
                return 0;
            } else {
                throw std::runtime_error("Unknown option: " + option);
            }
        }

        if (width <= 0 || height <= 0 ||
            !(focalLength > 0) || !(camWidth > 0) || depth < 0) {
            throw std::runtime_error(
                "Image dimensions and camera sizes must be positive; "
                "depth must be nonnegative."
            );
        }

        if (viewDirection.x() == 0 &&
            viewDirection.y() == 0 &&
            viewDirection.z() == 0) {
            throw std::runtime_error("View direction cannot be zero.");
        }

        if (!inputFile.empty()) {
            throw std::runtime_error(
                "--input is not connected yet. "
                "Omit it to render the mirror-box scene."
            );
        }

        frameBuffer fb(width, height);

        PerspectiveCamera camera(
            viewDirection,
            position,
            fb.getWidth(),
            fb.getHeight(),
            focalLength,
            camWidth
        );

        // Each material stores its own color.
        Lambertian lambertian(vec3(0.8, 0.15, 0.1));

        BlinnPhong blinnPhong(
            vec3(0.15, 0.3, 0.85),  // Diffuse color
            vec3(1, 1, 1)          // Shininess
        );

        blinnPhong.setP(100.0);

        Normal normal;
        Mirror mirror;

        Diffuse floorDiffuse(vec3(0.65, 0.65, 0.65));
        Diffuse ballDiffuse(vec3(0.15, 0.75, 0.3));

        // Box bounds: x [-3, 3], y [0, 5], z [-4, 4].
        // Triangle normals point into the box.
        Triangle floorTriangles[] = {
            Triangle(
                vec3(-3, 0, 4),
                vec3(3, 0, 4),
                vec3(3, 0, -4)
            ),
            Triangle(
                vec3(-3, 0, 4),
                vec3(3, 0, -4),
                vec3(-3, 0, -4)
            )
        };

        Triangle mirrorTriangles[] = {
            // Back wall: normal +Z
            Triangle(
                vec3(-3, 0, -4),
                vec3(3, 0, -4),
                vec3(3, 5, -4)
            ),
            Triangle(
                vec3(-3, 0, -4),
                vec3(3, 5, -4),
                vec3(-3, 5, -4)
            ),

            // Front wall: normal -Z
            Triangle(
                vec3(-3, 0, 4),
                vec3(-3, 5, 4),
                vec3(3, 5, 4)
            ),
            Triangle(
                vec3(-3, 0, 4),
                vec3(3, 5, 4),
                vec3(3, 0, 4)
            ),

            // Left wall: normal +X
            Triangle(
                vec3(-3, 0, 4),
                vec3(-3, 0, -4),
                vec3(-3, 5, -4)
            ),
            Triangle(
                vec3(-3, 0, 4),
                vec3(-3, 5, -4),
                vec3(-3, 5, 4)
            ),

            // Right wall: normal -X
            Triangle(
                vec3(3, 0, -4),
                vec3(3, 0, 4),
                vec3(3, 5, 4)
            ),
            Triangle(
                vec3(3, 0, -4),
                vec3(3, 5, 4),
                vec3(3, 5, -4)
            ),

            // Ceiling: normal -Y
            Triangle(
                vec3(-3, 5, -4),
                vec3(3, 5, -4),
                vec3(3, 5, 4)
            ),
            Triangle(
                vec3(-3, 5, -4),
                vec3(3, 5, 4),
                vec3(-3, 5, 4)
            )
        };

        const double radius = 0.38;

        Sphere lambertianBall(vec3(-2.0, radius, -1.4), radius);
        Sphere blinnPhongBall(vec3(-1.0, radius, -1.4), radius);
        Sphere normalBall(vec3(0.0, radius, -1.4), radius);
        Sphere mirrorBall(vec3(1.0, radius, -1.4), radius);
        Sphere diffuseBall(vec3(2.0, radius, -1.4), radius);

        PointLight light;
        light.position = vec3(-0.8, 3.8, 1.0);
        light.intesity = vec3(0.9, 0.9, 0.9);

        Scene scene;
        scene.addCam(camera);
        scene.addLight(light);
        scene.setBackground(vec3(0, 0, 0)); 

        for (Triangle& triangle : floorTriangles) {
            scene.addObject(triangle, floorDiffuse);
        }

        for (Triangle& triangle : mirrorTriangles) {
            scene.addObject(triangle, mirror);
        }

        scene.addObject(lambertianBall, lambertian);
        scene.addObject(blinnPhongBall, blinnPhong);
        scene.addObject(normalBall, normal);
        scene.addObject(mirrorBall, mirror);
        scene.addObject(diffuseBall, ballDiffuse);

        rayTracer renderer;
        renderer.render(scene, fb, 0, depth);

        fb.exportPNG(outputFile);

        std::cout << "Rendered mirror box to "
                  << outputFile << '\n';

        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}