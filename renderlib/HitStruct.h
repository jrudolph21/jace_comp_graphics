#pragma once
#include "ray.h"

class Shape;
class Shader;
class Scene;

struct HitStruct {
    double t = 0.0;
    vec3 intersectionPoint;
    vec3 normal;
    vec3 viewDir;
    const Shape* shape = nullptr;
    const Shader* shader = nullptr;
    const Scene* scene = nullptr;
};