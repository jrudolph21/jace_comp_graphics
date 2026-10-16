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
        viewDirection,
        position,
        fb.getWidth(),
        fb.getHeight(),
        focalLength,
        camWidth
    );

    Sphere greenSphere(vec3(-2,0.8,-3), 0.8);
    Sphere redSphere(vec3(-1.1, 0.6, -1.2), 0.6);
    Sphere normalSphere(vec3(0,0.9,-3.5), 0.9);
    Sphere blueSphere(vec3(0.3, 0.45, -1.6), 0.45);
    Sphere mirrorSphere(vec3(2, 1, -3), 1);
    Sphere frontMirrorSphere(vec3(1.5, 0.5, 0.3), 0.5);
    Sphere orangeSphere(vec3(0.3, 1, 5), 1);
    Sphere purpleSphere(vec3(2.4, 0.8, 4.8), 0.8);
    Sphere diffuseSphere(vec3(-1.3, 0.45, 0.4), 0.45);

    
    Triangle ground1(
        vec3(-50, 0, 50),
        vec3(50, 0, 50),
        vec3(50, 0, -50)
    );
    
    Triangle ground2(
        vec3(-50, 0, 50),
        vec3(50, 0, -50),
        vec3(-50, 0, -50)
    );

    Triangle wall1(
        vec3(-4, 0, -6),
        vec3(4, 0, -6),
        vec3(4, 4, -6)
    );

    Triangle wall2(
        vec3(-4, 0, -6),
        vec3(4, 4, -6),
        vec3(-4, 4, -6)
    );

    Triangle floatingTriangle(
        vec3(0.9, 1.4, -0.6),
        vec3(1.9, 1.4, -0.6),
        vec3(1.4, 2.2, -1.1)
    );

    
    Lambertian lambertian;
    BlinnPhong redBlinnPhong;
    BlinnPhong blueBlinnPhong;
    BlinnPhong orangeBlinnPhong;
    Diffuse diffuse;
    Normal normal;
    Mirror mirror;

    double redP = 64.0;
    double blueP = 16.0;
    double orangeP = 32.0;

    redBlinnPhong.setKd(vec3(1,1,1));
    redBlinnPhong.setP(redP);

    blueBlinnPhong.setKd(vec3(1,1,1));
    blueBlinnPhong.setP(blueP);

    orangeBlinnPhong.setKd(vec3(1,1,1));
    orangeBlinnPhong.setP(orangeP);

    PointLight light1;
    light1.position = vec3(-5, 8, 5);
    light1.intesity = vec3(.7, .6, .45);

    PointLight light2;
    light2.position = vec3(6, 6, -1);
    light2.intesity = vec3(.35, .45, .7);

    
    Scene scene;

    scene.addCam(camera);
    scene.addLight(light1);
    scene.addLight(light2);

    scene.setBackground(vec3(0.5, 0.7, 1));

    scene.addObject(
        floatingTriangle, lambertian, vec3(0.9, 0.8, 0.1)
    );

    scene.addObject(
        ground1, lambertian, vec3(0.6, 0.6, 0.6)
    );

    scene.addObject(
        ground2, lambertian, vec3(0.6, 0.6, 0.6)
    );

    scene.addObject(
        wall1, mirror, vec3(1, 1, 1)
    );

    scene.addObject(
        wall2, mirror, vec3(1, 1, 1)
    );

    scene.addObject(
        greenSphere, lambertian, vec3(0.2, 0.8, 0.2)
    );  

    scene.addObject(
        diffuseSphere, diffuse, vec3(.2, .7, .7)
    );
    
    scene.addObject(
        purpleSphere, lambertian, vec3(0.6, 0.2, 0.8)
    );   

    scene.addObject(
        redSphere, redBlinnPhong, vec3(0.8, 0.1, 0.1)
    );

    scene.addObject(
        blueSphere, blueBlinnPhong, vec3(0.1, 0.2, 0.8)
    );

    scene.addObject(
        orangeSphere, orangeBlinnPhong, vec3(0.95, 0.5, 0.1)
    );


    scene.addObject(
        normalSphere, normal, vec3(1,1,1)
    );

    scene.addObject(
        mirrorSphere, mirror, vec3(1,1,1)
    );

    scene.addObject(
        frontMirrorSphere, mirror, vec3(1,1,1)
    );

    // Render the scene 
    rayTracer renderer;
    renderer.render(scene, fb, 0, depth);

    // Save the image
    fb.exportPNG(outputFile);

    std::cout << "Rendered scene to " << outputFile << '\n';
    return 0;
}
