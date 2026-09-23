#pragma once
#include "ray.h"

class Shape {

    public:

        virtual ~Shape() = default;

        virtual bool intersect(const ray &r) = 0;

};

class Sphere : Shape {

    public:

        Sphere();

        Sphere(vec3 center, double radius);

        bool intersect(const ray &r) override;

        vec3 getCenter();

        double getRadius();

    private:

        vec3 center;
        double radius;

};