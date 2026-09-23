#include "shape.h"

Sphere::Sphere() : center(0, 0, -1), radius(1.0) {}

Sphere::Sphere(vec3 center, double radius) : center(center), radius(radius) {}

vec3 Sphere::getCenter() {
    return center;
}

double Sphere::getRadius() {
    return radius;
}

bool Sphere::intersect(const ray &r) {

    vec3 rd = r.getDirection();
    vec3 ro = r.getOrigin();

    vec3 temp = ro - center;

    double A = dot(rd, rd);
    double B = dot(2*rd, temp);
    double C = dot(temp, temp) - radius*radius;

    double D = B*B - 4*A*C;

    return (D >= 0);

}
