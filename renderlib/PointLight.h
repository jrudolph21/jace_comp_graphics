#pragma once
#include "ray.h"

struct PointLight {
    vec3 position;
    vec3 intesity = vec3(1.0, 1.0, 1.0);
};