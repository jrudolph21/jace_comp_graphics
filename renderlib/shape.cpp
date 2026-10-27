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

aabb Sphere::boundingBox() const {

    vec3 outer_edge(radius, radius, radius);

    return aabb(center - outer_edge, center + outer_edge);

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

bool Triangle::mollerIntersect(const ray &r, const float tmin, float &tmax, HitStruct &hit) {

    constexpr float epsilon = std::numeric_limits<float>::epsilon();

    vec3 rd = r.getDirection();
    vec3 ro = r.getOrigin();

    vec3 A = getPointA();
    vec3 B = getPointB();
    vec3 C = getPointC();

    vec3 AedgeBA = B - A;
    vec3 AedgeCA = C - A;

    const vec3 normal = cross(AedgeBA, AedgeCA);

    vec3 rdCrossEdgeCA = cross(rd, AedgeCA);
  

    float det = dot(AedgeBA, rdCrossEdgeCA);

    double invDet = 1.0 / det;

    if (std::abs(det) < epsilon) {
        return false;
    }

    vec3 s = ro - A;

    float u = ( dot(s, rdCrossEdgeCA) ) * invDet;

    if( u < -epsilon || u - 1 > epsilon) {
        return false;
    }

    vec3 sCrossEdgeBA = cross(s, AedgeBA);

    float v = ( dot(rd, sCrossEdgeBA) ) * invDet;

    if( v < -epsilon || u + v - 1 > epsilon) {
        return false;
    }

    float t = ( dot(AedgeCA,sCrossEdgeBA) ) * invDet;

    if (t < tmin || t > tmax) {
        return false;
    }
    else {
        hit.t = t;
        hit.intersectionPoint = r.at(t);
        hit.normal = normal / normal.length();
        hit.shape = this;
        tmax = static_cast<float>(t);
        return true;
    }

}

aabb Triangle::boundingBox() const {

    const vec3 minimum(
        std::min({pointA.x(), pointB.x(), pointC.x()}),
        std::min({pointA.y(), pointB.y(), pointC.y()}),
        std::min({pointA.z(), pointB.z(), pointC.z()}));

    const vec3 maximum(
        std::max({pointA.x(), pointB.x(), pointC.x()}),
        std::max({pointA.y(), pointB.y(), pointC.y()}),
        std::max({pointA.z(), pointB.z(), pointC.z()}));

    return aabb(minimum, maximum);
}

bvhNode::bvhNode(std::vector<Shape*> shapes, int axis) {
    const auto N = shapes.size();

    if (N == 0) {
        throw std::invalid_argument(
            "Cannot build a BVH from an empty shape list");
    }

    if (axis < 0 || axis > 2) {
        throw std::invalid_argument(
            "BVH axis must be 0, 1, or 2");
    }

    for (Shape* shape : shapes) {
        if (shape == nullptr) {
            throw std::invalid_argument(
                "BVH shape list contains a null pointer");
        }
    }

    if (N == 1) {
        leftChild = shapes[0];
        rightChild = nullptr;

        bbox = leftChild->boundingBox();
        return;
    }
    else if (N == 2) {
        leftChild = shapes[0];
        rightChild = shapes[1];
    }
    else {

        std::sort(
            shapes.begin(),
            shapes.end(),
            [axis](const Shape* a, const Shape* b) {

                const aabb boxA = a->boundingBox();
                const aabb boxB = b->boundingBox();

                const vec3 centerA =
                    0.5 * boxA.minimum + 0.5 * boxA.maximum;

                const vec3 centerB =
                    0.5 * boxB.minimum + 0.5 * boxB.maximum;

                if (axis == 0) {
                    return centerA.x() < centerB.x();
                }

                if (axis == 1) {
                    return centerA.y() < centerB.y();
                }

                return centerA.z() < centerB.z();
            });

        const auto middle = shapes.begin() + N / 2;

        std::vector<Shape*> leftside(shapes.begin(), middle);

        std::vector<Shape*> rightside(middle, shapes.end());

        const int nextAxis = (axis + 1) % 3;

        leftSubtree = std::make_unique<bvhNode>(leftside, nextAxis);

        rightSubtree = std::make_unique<bvhNode>(rightside, nextAxis);

        leftChild = leftSubtree.get();
        rightChild = rightSubtree.get();
    }

    bbox = mergeBoundingBoxes(
        leftChild->boundingBox(),
        rightChild->boundingBox());
}

bool bvhNode::intersect(
    const ray& r,
    const float tmin,
    float& tmax,
    HitStruct& hit)
{
    float boxTmax = tmax;

    if (!bbox.hit(r, tmin, boxTmax)) {
        return false;
    }

    bool hitLeft = false;
    bool hitRight = false;

    if (leftChild) {
        hitLeft = leftChild->intersect(r, tmin, tmax, hit);
    }

    if (rightChild) {
        hitRight = rightChild->intersect(r, tmin, tmax, hit);
    }

    return hitLeft || hitRight;
}


aabb bvhNode::mergeBoundingBoxes(
    const aabb& a,
    const aabb& b)
{
    aabb result;

    result.minimum = vec3(
        std::min(a.minimum.x(), b.minimum.x()),
        std::min(a.minimum.y(), b.minimum.y()),
        std::min(a.minimum.z(), b.minimum.z()));

    result.maximum = vec3(
        std::max(a.maximum.x(), b.maximum.x()),
        std::max(a.maximum.y(), b.maximum.y()),
        std::max(a.maximum.z(), b.maximum.z()));

    return result;
}

aabb bvhNode::boundingBox() const {
    return bbox;
}