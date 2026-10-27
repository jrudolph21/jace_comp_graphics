#pragma once
#include "ray.h"
#include "AABB.h"
#include "HitStruct.h"
#include <vector>
#include <cmath>

class Shape {

    public:

        virtual ~Shape() = default;

        virtual bool intersect(const ray &r, const float tmin, float &tmax, HitStruct &hit) = 0;

        virtual aabb boundingBox() const = 0;

};

class Sphere : public Shape {

    public:

        Sphere();

        Sphere(vec3 center, double radius);

        bool intersect(const ray &r, const float tmin, float &tmax, HitStruct &hit) override;

        vec3 getCenter();

        double getRadius();

        aabb boundingBox() const override;

    private:

        vec3 center;
        double radius;

};

class Triangle : public Shape {

    public:

        Triangle();

        Triangle(vec3 a, vec3 b, vec3 c);

        bool intersect(const ray &r, const float tmin, float &tmax, HitStruct &hit) override;
        bool mollerIntersect(const ray &r, const float tmin, float &tmax, HitStruct &hit);

        vec3 getPointA();
        vec3 getPointB();
        vec3 getPointC();

        aabb boundingBox() const override;

    private:
        vec3 pointA;
        vec3 pointB;
        vec3 pointC;

};

class bvhNode : public Shape {
    
    public:
        bvhNode(std::vector<Shape*> shapes, int axis = 0);

        bool intersect(
            const ray& r,
            const float tmin,
            float& tmax,
            HitStruct& hit) override;

        aabb boundingBox() const override;

    private:

        Shape* leftChild = nullptr;
        Shape* rightChild = nullptr;

    
        std::unique_ptr<bvhNode> leftSubtree;
        std::unique_ptr<bvhNode> rightSubtree;

        aabb bbox;

        aabb mergeBoundingBoxes(const aabb& a, const aabb& b);
};