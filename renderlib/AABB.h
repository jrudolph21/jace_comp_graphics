#pragma once

#include <algorithm>
#include <limits>
#include "ray.h"

class aabb {
public:
    vec3 minimum;
    vec3 maximum;

    aabb()
        : minimum(
              std::numeric_limits<double>::infinity(),
              std::numeric_limits<double>::infinity(),
              std::numeric_limits<double>::infinity()),
          maximum(
              -std::numeric_limits<double>::infinity(),
              -std::numeric_limits<double>::infinity(),
              -std::numeric_limits<double>::infinity())
    {
    }

    aabb(const vec3& a, const vec3& b)
        : minimum(
              std::min(a.x(), b.x()),
              std::min(a.y(), b.y()),
              std::min(a.z(), b.z())),
          maximum(
              std::max(a.x(), b.x()),
              std::max(a.y(), b.y()),
              std::max(a.z(), b.z()))
    {
    }

    bool hit(const ray& r, double tmin, double tmax) const {
        const vec3 origin = r.getOrigin();
        const vec3 direction = r.getDirection();

        // Local arrays let us loop over axes without indexing vec3.
        const double origins[3] = {
            origin.x(), origin.y(), origin.z()
        };

        const double directions[3] = {
            direction.x(), direction.y(), direction.z()
        };

        const double mins[3] = {
            minimum.x(), minimum.y(), minimum.z()
        };

        const double maxs[3] = {
            maximum.x(), maximum.y(), maximum.z()
        };

        if (tmax < tmin) {
            return false;
        }

        for (int axis = 0; axis < 3; ++axis) {
            if (mins[axis] > maxs[axis]) {
                return false;
            }

            // Parallel to this pair of boundary planes.
            if (directions[axis] == 0.0) {
                if (origins[axis] < mins[axis] ||
                    origins[axis] > maxs[axis]) {
                    return false;
                }

                continue;
            }

            double t0 =
                (mins[axis] - origins[axis]) / directions[axis];

            double t1 =
                (maxs[axis] - origins[axis]) / directions[axis];

            if (t0 > t1) {
                std::swap(t0, t1);
            }

            tmin = std::max(tmin, t0);
            tmax = std::min(tmax, t1);

            if (tmax < tmin) {
                return false;
            }
        }

        return true;
    }
};