#include "rayTracer.h"
#include <iostream>
#include <string>

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
    int depth = 3;
    vec3 position(0, 0, 0);
    vec3 viewDirection(0, 0, -1);
    double focalLength = 1.0;
    double camWidth = 0.5;
    std::string outputFile = "render.png";

   
    for (int i = 1; i < argc; ++i) {
        std::string option = argv[i];

        if (option == "--width") {
            width = std::stoi(argv[++i]);
        } else if (option == "--height") {
            height = std::stoi(argv[++i]);
        } else if (option == "--depth") {
            depth = std::stoi(argv[++i]);
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

    PerspectiveCamera camera(
        viewDirection,
        position,
        fb.getWidth(),
        fb.getHeight(),
        focalLength,
        camWidth
    );

    
    Sphere redSphere(vec3(-1.2, 1.0, -3.0), 1.0);
    Sphere mirrorSphere(vec3(1.2, 1.10, -4.0), 1.10);

    
    Triangle ground(
        vec3(-100, 0, 100),
        vec3(100, 0, 100),
        vec3(0, 0, -100)
    );

    
    Lambertian lambertian;
    BlinnPhong blinnPhong;
    blinnPhong.setP(100.0);
    blinnPhong.setKd(vec3(1, 1, 1));
    Mirror mirror;

    PointLight light1;
    light1.position = vec3(-3, 5, 1);

    PointLight light2;
    light2.position = vec3(4, 5, -1);

    
    Scene scene;

    scene.addCam(camera);
    scene.addLight(light1);
    scene.addLight(light2);

    scene.setBackground(vec3(0.69, 0.88, 0.98));

    scene.addObject(
        ground, lambertian, vec3(0.7, 0.7, 0.7)
    );

    scene.addObject(
        redSphere, blinnPhong, vec3(1.0, 0.0, 0.0)
    );

    scene.addObject(
        mirrorSphere, mirror, vec3(1.0, 1.0, 1.0)
    );
    // Render the scene 
    rayTracer renderer;
    renderer.render(scene, fb, 0, depth);

    // Save the image
    fb.exportPNG(outputFile);

    std::cout << "Rendered scene to " << outputFile << '\n';
    return 0;
}
