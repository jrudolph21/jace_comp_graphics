#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    public:

    ray() {};

    ray(const vec3 &orig, const vec3 &dir) {
        direction = dir;
        origin = orig;
    }

    vec3 getDirection() const {
        return direction;
    }

    vec3 getOrigin() const {
        return origin;
    }

    vec3 at(double t) const {
        return origin + (direction * t);
    }

    private:
        vec3 direction;
        vec3 origin;

};

#endif