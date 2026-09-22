#pragma once

#include "ray.h"

class Camera {
    public:

        ~Camera();

        Camera();

        virtual ray generateRay(double x, double y) = 0;

    protected:

    vec3 U, V, W;
    vec3 origin;
};

class PerspectiveCamera : Camera {

    public:

        ~PerspectiveCamera();

        PerspectiveCamera();

        PerspectiveCamera(int imageWidth, int imageHeight, double focalLength, double camWidth);

        PerspectiveCamera(vec3 viewDirection, vec3 postition, int imageWidth, int imageHeight, double focalLength, double camWidth);

        ray generateRay(double x, double y) override;

    private:

        void updateCameraFrame();
        
        int imageWidth;
        int imageHeight;
        double focalLength;
        double camWidth;
        vec3 viewDirection;
        vec3 position;

};