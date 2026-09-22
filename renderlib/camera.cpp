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
    this->position = vec3(0, 0, 0);
    this->viewDirection = vec3(0, 0, -1);
    updateCameraFrame();
}

PerspectiveCamera::PerspectiveCamera(vec3 viewDirection, vec3 position, int imageWidth, int imageHeight, double focalLength, double camWidth) : Camera() {
    this->imageWidth = imageWidth;
    this->imageHeight = imageHeight;
    this->focalLength = focalLength;
    this->camWidth = camWidth;
    this->viewDirection = viewDirection;
    this->position = position;
    updateCameraFrame();
}

void PerspectiveCamera::updateCameraFrame() {
    origin = position;

    // W faces opposite of viewDirection
    vec3 w_dir = viewDirection * -1.0;
    W = w_dir / w_dir.length();

    // Default world up vector
    vec3 up(0, 1, 0);
    
    // Fallback if viewDirection is parallel to world up vector
    if (std::abs(W.x()) < 1e-6 && std::abs(W.z()) < 1e-6) {
        up = vec3(0, 0, 1);
    }

    vec3 u_dir = cross(up, W);
    U = u_dir / u_dir.length();

    V = cross(W, U);
}

ray PerspectiveCamera::generateRay(double x, double y) {

    double camHeight = camWidth * static_cast<double>(imageHeight) / imageWidth;

    double left_b = -camWidth / 2.0;
    double right_b = camWidth / 2.0;
    double top_b = camHeight / 2.0;
    double bottom_b = -camHeight / 2.0;

    double u = left_b + (right_b - left_b) * ((x + 0.5) / imageWidth);
    double v = bottom_b + (top_b - bottom_b) * ((y + 0.5) / imageHeight);

    vec3 direction = U * u + V * v - W * focalLength;
    return ray(origin, direction);
}