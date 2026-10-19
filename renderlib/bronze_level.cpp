#include "rayTracer.h"
#include <iostream>
#include <string>

// ./renderlib/Debug/frameBuffer.exe --width 500 --height 500 --position 0 3 3 --direction 0 -0.4 -1 --focal-length 0.4 --cam-width 0.5 --depth 5 --output render.png

void printUsage_lab(const char* program) {
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
            printUsage_lab(argv[0]);
            return 0;
        }
    }


    frameBuffer fb(width, height);

    PerspectiveCamera camera(
        vec3(0, -0.22, -1),       
        vec3(0, 1.8, 3.0),        
        fb.getWidth(),
        fb.getHeight(),
        0.4,                     
        0.5                      
    );

    // Materials
    Lambertian lambertian;
    BlinnPhong blinnPhong;
    Normal normal;
    Mirror mirror;
    Diffuse diffuse;

    blinnPhong.setKd(vec3(1, 1, 1));
    blinnPhong.setP(100.0);

    // Make sure Diffuse's constructor initializes kd to (1,1,1).
    // If you have a setter, you can also use:
    // diffuse.setKd(vec3(1, 1, 1));

    // Box bounds:
    // x: -3 to 3
    // y:  0 to 5
    // z: -4 to 4
    //
    // Triangle winding makes all normals point INTO the box.

    Triangle floorTriangles[] = {
        Triangle(
            vec3(-3, 0,  4),
            vec3( 3, 0,  4),
            vec3( 3, 0, -4)
        ),
        Triangle(
            vec3(-3, 0,  4),
            vec3( 3, 0, -4),
            vec3(-3, 0, -4)
        )
    };

    Triangle mirrorTriangles[] = {
        // Back wall: normal +Z
        Triangle(
            vec3(-3, 0, -4),
            vec3( 3, 0, -4),
            vec3( 3, 5, -4)
        ),
        Triangle(
            vec3(-3, 0, -4),
            vec3( 3, 5, -4),
            vec3(-3, 5, -4)
        ),

        // Front wall, behind the camera: normal -Z
        Triangle(
            vec3(-3, 0, 4),
            vec3(-3, 5, 4),
            vec3( 3, 5, 4)
        ),
        Triangle(
            vec3(-3, 0, 4),
            vec3( 3, 5, 4),
            vec3( 3, 0, 4)
        ),

        // Left wall: normal +X
        Triangle(
            vec3(-3, 0,  4),
            vec3(-3, 0, -4),
            vec3(-3, 5, -4)
        ),
        Triangle(
            vec3(-3, 0,  4),
            vec3(-3, 5, -4),
            vec3(-3, 5,  4)
        ),

        // Right wall: normal -X
        Triangle(
            vec3(3, 0, -4),
            vec3(3, 0,  4),
            vec3(3, 5,  4)
        ),
        Triangle(
            vec3(3, 0, -4),
            vec3(3, 5,  4),
            vec3(3, 5, -4)
        ),

        // Ceiling: normal -Y
        Triangle(
            vec3(-3, 5, -4),
            vec3( 3, 5, -4),
            vec3( 3, 5,  4)
        ),
        Triangle(
            vec3(-3, 5, -4),
            vec3( 3, 5,  4),
            vec3(-3, 5,  4)
        )
    };

    // Five small balls resting on the floor.
    const double radius = 0.38;

    Sphere lambertianBall(vec3(-2.0, radius, -1.4), radius);
    Sphere blinnPhongBall(vec3(-1.0, radius, -1.4), radius);
    Sphere normalBall    (vec3( 0.0, radius, -1.4), radius);
    Sphere mirrorBall    (vec3( 1.0, radius, -1.4), radius);
    Sphere diffuseBall   (vec3( 2.0, radius, -1.4), radius);

    // One point light INSIDE the box.
    PointLight light;
    light.position = vec3(-0.8, 3.8, 1.0);
    light.intesity = vec3(0.9, 0.9, 0.9);

    Scene scene;
    scene.addCam(camera);
    scene.addLight(light);
    scene.setBackground(vec3(0, 0, 0));

    // Light gray diffuse floor.
    for (Triangle& triangle : floorTriangles) {
        scene.addObject(
            triangle, diffuse, vec3(0.65, 0.65, 0.65)
        );
    }

    // Mirror walls and ceiling.
    for (Triangle& triangle : mirrorTriangles) {
        scene.addObject(
            triangle, mirror, vec3(1, 1, 1)
        );
    }

    // Left to right: Lambertian, Blinn-Phong, Normal, Mirror, Diffuse.
    scene.addObject(
        lambertianBall, lambertian, vec3(0.8, 0.15, 0.1)
    );

    scene.addObject(
        blinnPhongBall, blinnPhong, vec3(0.15, 0.3, 0.85)
    );

    scene.addObject(
        normalBall, normal, vec3(1, 1, 1)
    );

    scene.addObject(
        mirrorBall, mirror, vec3(1, 1, 1)
    );

    scene.addObject(
        diffuseBall, diffuse, vec3(0.15, 0.75, 0.3)
    );

    rayTracer renderer;
    renderer.render(scene, fb, 0, depth);

    fb.exportPNG(outputFile);

    std::cout << "Rendered mirror box to "
              << outputFile << '\n';

    return 0;
}