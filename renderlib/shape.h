#pragma once
#include "ray.h"
#include "HitStruct.h"

class Shape {

    public:

        virtual ~Shape() = default;

        virtual bool intersect(const ray &r, const float tmin, float &tmax, HitStruct &hit) = 0;

};

class Sphere : public Shape {

    public:

        Sphere();

        Sphere(vec3 center, double radius);

        bool intersect(const ray &r, const float tmin, float &tmax, HitStruct &hit) override;

        vec3 getCenter();

        double getRadius();

    private:

        vec3 center;
        double radius;

};

class Triangle : public Shape {

    public:

        Triangle();

        Triangle(vec3 a, vec3 b, vec3 c);

        bool intersect(const ray &r, const float tmin, float &tmax, HitStruct &hit) override;

        vec3 getPointA();
        vec3 getPointB();
        vec3 getPointC();

    private:
        vec3 pointA;
        vec3 pointB;
        vec3 pointC;

};