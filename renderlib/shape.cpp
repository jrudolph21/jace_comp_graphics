#include "shape.h"

Sphere::Sphere() : center(0, 0, -1), radius(1.0) {}

Sphere::Sphere(vec3 center, double radius) : center(center), radius(radius) {}

vec3 Sphere::getCenter() {
    return center;
}

double Sphere::getRadius() {
    return radius;
}

bool Sphere::intersect(const ray &r, const float tmin, float &tmax, HitStruct &hit) {

    vec3 rd = r.getDirection();
    vec3 ro = r.getOrigin();

    vec3 temp = ro - center;

    double A = dot(rd, rd);
    double B = dot(2*rd, temp);
    double C = dot(temp, temp) - radius*radius;

    double D = B*B - 4*A*C;

    if (D < 0.0) {
        return false;
    }

    double t = (-B - std::sqrt(D)) / (2.0 * A);

    if (t < tmin || t > tmax) {
        t = (-B + std::sqrt(D)) / (2.0 * A);
        if (t < tmin || t > tmax) {
            return false;
        }
    }

    // Only update the record when this sphere has a valid hit.
    hit.t = t;
    hit.intersectionPoint = r.at(t);
    hit.normal = (1.0 / radius) * (hit.intersectionPoint - center);
    hit.shape = this;
    tmax = static_cast<float>(t);
    return true;

}

Triangle::Triangle() : pointA(vec3(1,0,0)), pointB(vec3(0,1,0)), pointC(vec3(0,0,1)) {}

Triangle::Triangle(vec3 a, vec3 b, vec3 c) : pointA(a), pointB(b), pointC(c) {}

vec3 Triangle::getPointA() {
    return pointA;
}

vec3 Triangle::getPointB() {
    return pointB;
}

vec3 Triangle::getPointC() {
    return pointC;
}

bool Triangle::intersect(const ray &r, const float tmin, float &tmax, HitStruct &hit) {

    vec3 rd = r.getDirection();
    vec3 ro = r.getOrigin();

    vec3 A = getPointA();
    vec3 B = getPointB();
    vec3 C = getPointC();

    // Using Cramer's Rule

    double a = A.x() - B.x();
    double b = A.y() - B.y();
    double c = A.z() - B.z();
    double d = A.x() - C.x();
    double e = A.y() - C.y();
    double f = A.z() - C.z();
    double g = rd.x();
    double h = rd.y();
    double i = rd.z();
    double j = A.x() - ro.x();
    double k = A.y() - ro.y();
    double l = A.z() - ro.z();

    double eihf = (e*i) - (h*f);
    double gfdi = (g*f) - (d*i);
    double dheg = (d*h) - (e*g);
    double akjb = (a*k) - (j*b);
    double jcal = (j*c) - (a*l);
    double blkc = (b*l) - (k*c);

    double M = a*eihf + b*gfdi + c*dheg;

    double beta = (j*eihf + k*gfdi + l*dheg) / M;
    double gamma = (i*akjb + h*jcal + g*blkc) / M;
    double t = -(f*akjb + e*jcal + d*blkc) / M;
    
    if (t < tmin || t > tmax) {
        return false;
    }

    if (gamma < 0 || gamma > 1) {
        return false;
    }

    if (beta < 0 || beta > 1 - gamma) {
        return false;
    }

    hit.t = t;
    hit.intersectionPoint = r.at(t);
    vec3 test(cross((B-A), (C-A)));
    hit.normal = test / test.length();
    hit.shape = this;
    tmax = static_cast<float>(t);
    return true;

}