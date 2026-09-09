#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class vec3 {
    public:
        double coords[3];

        vec3() {
            coords[0] = 0.0;
            coords[1] = 0.0;
            coords[2] = 0.0;
        }

        vec3(double x, double y, double z) {
            coords[0] = x;
            coords[1] = y;
            coords[2] = z;
        }

        double x() const {
            return coords[0];
        }

        double y() const {
            return coords[1];
        }

        double z() const {
            return coords[2];
        }

        vec3& operator+=(const vec3& v) {
            coords[0] += v.coords[0];
            coords[1] += v.coords[1];
            coords[2] += v.coords[2];
            return *this;
        }

        vec3& operator*=(double t) {
            coords[0] *= t;
            coords[1] *= t;
            coords[2] *= t;
            return *this;
        }

        vec3& operator/=(double t) {
            return *this *= 1/t;
        }

        double length() const {
            return std::sqrt(length_squared());
        }

        double length_squared() const {
            return coords[0]*coords[0] + coords[1]*coords[1] + coords[2]*coords[2];
        }

    };

    inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
        return out << v.x() << ' ' << v.y() << ' ' << v.z();
    }

    inline vec3 operator+(const vec3& u, const vec3& v) {
        return vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
    }

    inline vec3 operator-(const vec3& u, const vec3& v) {
        return vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
    }

    inline vec3 operator*(const vec3& u, const vec3& v) {
        return vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
    }

    inline vec3 operator*(double t, const vec3& v) {
        return vec3(t*v.x(), t*v.y(), t*v.z());
    }

    inline vec3 operator*(const vec3& v, double t) {
        return t * v;
    }

    inline vec3 operator/(const vec3& v, double t) {
        return (1/t) * v;
    }

    inline double dot(const vec3& u, const vec3& v) {
        return u.x() * v.x()
             + u.y() * v.y()
             + u.z() * v.z();
    }

    inline vec3 cross(const vec3& u, const vec3& v) {
        return vec3(u.y() * v.z() - u.z() * v.y(),
                    u.z() * v.x() - u.x() * v.z(),
                    u.x() * v.y() - u.y() * v.x());
    }

    inline vec3 unit_vec(const vec3& v) {
        return v / v.length();
    }
    
#endif
