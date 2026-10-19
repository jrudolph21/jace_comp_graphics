#include "rayTracer.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

class BenchmarkTriangle : public Triangle {
public:
    BenchmarkTriangle(vec3 a, vec3 b, vec3 c, const bool& useMoller)
        : Triangle(a, b, c), useMoller(useMoller) {}

    bool intersect(const ray& r, const float tmin, float& tmax,
                   HitStruct& hit) override {
        if (useMoller) {
            return Triangle::mollerIntersect(r, tmin, tmax, hit);
        }
        return Triangle::intersect(r, tmin, tmax, hit);
    }

private:
    const bool& useMoller;
};

void printUsage(const char* program) {
    std::cout << "Usage: " << program << " [options]\n"
              << "  --width N           Image width (default 800)\n"
              << "  --height N          Image height (default 800)\n"
              << "  --position X Y Z    Camera position (default 0 0 0)\n"
              << "  --direction X Y Z   View direction (default 0 0 -1)\n"
              << "  --focal-length F    Focal length (default 1.0)\n"
              << "  --cam-width W       Camera plane width (default 0.5)\n"
              << "  --runs N            Timed renders per method (default 3)\n"
              << "  --output PREFIX     PNG prefix (default triangle)\n"
              << "  --help              Show this help\n";
}

int main(int argc, char* argv[]) {
    int width = 800;
    int height = 800;
    int runs = 1;
    vec3 position(0, 0, 0);
    vec3 viewDirection(0, 0, -1);
    double focalLength = 1.0;
    double camWidth = 0.5;
    std::string outputPrefix = "triangle";

    try {
        auto nextValue = [&](int& i) -> std::string {
            if (i + 1 >= argc) throw std::runtime_error("Missing option value.");
            return argv[++i];
        };
        for (int i = 1; i < argc; ++i) {
            const std::string option = argv[i];
            if (option == "--width") {
                width = std::stoi(nextValue(i));
            } else if (option == "--height") {
                height = std::stoi(nextValue(i));
            } else if (option == "--position" || option == "--direction") {
                const double x = std::stod(nextValue(i));
                const double y = std::stod(nextValue(i));
                const double z = std::stod(nextValue(i));
                if (!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z))
                    throw std::runtime_error("Camera values must be finite.");
                if (option == "--position") position = vec3(x, y, z);
                else viewDirection = vec3(x, y, z);
            } else if (option == "--focal-length") {
                focalLength = std::stod(nextValue(i));
            } else if (option == "--cam-width") {
                camWidth = std::stod(nextValue(i));
            } else if (option == "--runs") {
                runs = std::stoi(nextValue(i));
            } else if (option == "--output") {
                outputPrefix = nextValue(i);
            } else if (option == "--help" || option == "-h") {
                printUsage(argv[0]);
                return 0;
            } else {
                throw std::runtime_error("Unknown option: " + option);
            }
        }
        if (width <= 0 || height <= 0 || runs <= 0 ||
            !std::isfinite(focalLength) || focalLength <= 0 ||
            !std::isfinite(camWidth) || camWidth <= 0 ||
            viewDirection.length() == 0) {
            throw std::runtime_error("Dimensions, runs, and camera sizes must be positive; direction must be nonzero.");
        }
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }

    frameBuffer oldImage(width, height);
    frameBuffer mollerImage(width, height);
    PerspectiveCamera camera(viewDirection, position, width, height,
                             focalLength, camWidth);


    bool useMoller = false;
    BenchmarkTriangle leftTriangle(
        vec3(-1.1, -0.7, -6), vec3(-0.1, -0.7, -6),
        vec3(-0.6, 0.7, -6), useMoller);
    BenchmarkTriangle rightTriangle(
        vec3(0.1, -0.7, -6), vec3(1.1, -0.7, -6),
        vec3(0.6, 0.7, -6), useMoller);

    Lambertian lambertian;
    BlinnPhong blinnPhong;

    PointLight light1;
    light1.position = vec3(-3, 5, -3);
    PointLight light2;
    light2.position = vec3(3, 5, -3);

    Scene scene;
    scene.addCam(camera);
    scene.addLight(light1);
    scene.addLight(light2);
    scene.setBackground(vec3(0, 0, 0));
    scene.addObject(leftTriangle, lambertian, vec3(1, 0, 0));
    scene.addObject(rightTriangle, blinnPhong, vec3(1, 0.5, 0));

    rayTracer renderer;

    useMoller = false;
    renderer.render(scene, oldImage);
    useMoller = true;
    renderer.render(scene, mollerImage);

    std::cout << "Here\n";

    auto timedRender = [&](bool moller, frameBuffer& image) {
        useMoller = moller;

        const auto start = std::chrono::steady_clock::now();
        renderer.render(scene, image);
        const auto end = std::chrono::steady_clock::now();
        return std::chrono::duration<double, std::milli>(end - start).count();
    };

    double oldTotal = 0.0;
    double mollerTotal = 0.0;
    for (int run = 0; run < runs; ++run) {
        double oldMs, mollerMs;
        if (run % 2 == 0) {
            oldMs = timedRender(false, oldImage);
            mollerMs = timedRender(true, mollerImage);
        } else {
            mollerMs = timedRender(true, mollerImage);
            oldMs = timedRender(false, oldImage);
        }
        oldTotal += oldMs;
        mollerTotal += mollerMs;
        std::cout << "Run " << run + 1 << ": old = " << oldMs
                  << " ms, Moller = " << mollerMs << " ms\n";
    }


    oldImage.exportPNG(outputPrefix + "_old.png");
    mollerImage.exportPNG(outputPrefix + "_moller.png");

    std::cout << "\nOld average render time: " << oldTotal / runs << " ms\n"
              << "Moller average render time: " << mollerTotal / runs << " ms\n";
    if (mollerTotal > 0.0) {
        std::cout << "Render speed ratio (old / Moller): "
                  << oldTotal / mollerTotal << "x\n";
    }
    std::cout << "Ratio > 1 means Moller rendered faster.\n"
              << "Saved " << outputPrefix << "_old.png and "
              << outputPrefix << "_moller.png\n"
              << "These are full render timings, including shading and sampling.\n";
    return 0;
}
