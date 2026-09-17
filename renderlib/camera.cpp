#include "camera.h"

Camera::~Camera() {}

Camera::Camera() {
    U = vec3(1, 0, 0);
    V = vec3(0, 1, 0);
    W = vec3(0, 0, 1);
    origin = vec3(0, 0, 0);
}

PerspectiveCamera::~PerspectiveCamera() {}

PerspectiveCamera::PerspectiveCamera() : Camera() {
    imageWidth = 512;
    imageHeight = 512;
    focalLength = 1.0;
    camWidth = 0.5;
}

PerspectiveCamera::PerspectiveCamera(int imageWidth, int imageHeight, double focalLength, double camWidth) : Camera() {
    this->imageWidth = imageWidth;
    this->imageHeight = imageHeight;
    this->focalLength = focalLength;
    this->camWidth = camWidth;
}

ray PerspectiveCamera::generateRay(double x, double y) {

    double camHeight = camWidth * static_cast<double>(imageHeight) / imageWidth;

    double left_b =  -camWidth / 2;
    double right_b = camWidth / 2;
    double top_b = camHeight / 2;
    double bottom_b = -camHeight / 2;

    double u = left_b + (right_b - left_b) * ((x + 0.5) / imageWidth);
    double v = bottom_b + (top_b - bottom_b) * ((y + 0.5) / imageHeight);



    vec3 direction = U * u + V * v - W * focalLength;
    return ray(origin, direction);
}