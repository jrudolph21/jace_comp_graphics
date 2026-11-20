#pragma once

#include <vector>
#include <glm/glm.hpp>

class OpenGLTriangle {
public:

    OpenGLTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) : a(a), b(b), c(c) { }


    void subdivide(int depth, float radius, std::vector<float>& vertices) const {
        
        const glm::vec3 unitA = glm::normalize(a);
        const glm::vec3 unitB = glm::normalize(b);
        const glm::vec3 unitC = glm::normalize(c);

        if (depth <= 0) {
            // For a sphere centered at the origin, the outward
            // normal is the normalized vertex position
            appendVertex(vertices, radius * unitA, unitA);
            appendVertex(vertices, radius * unitB, unitB);
            appendVertex(vertices, radius * unitC, unitC);
            return;
        }

        // Find edge midpoints and project them onto the unit sphere
        const glm::vec3 ab = glm::normalize(unitA + unitB);
        const glm::vec3 bc = glm::normalize(unitB + unitC);
        const glm::vec3 ca = glm::normalize(unitC + unitA);

        // Replace this triangle with four smaller triangles
        OpenGLTriangle(unitA, ab, ca).subdivide(depth - 1, radius, vertices);

        OpenGLTriangle(ab, unitB, bc).subdivide(depth - 1, radius, vertices);

        OpenGLTriangle(ca, bc, unitC).subdivide(depth - 1, radius, vertices);

        OpenGLTriangle(ab, bc, ca).subdivide(depth - 1, radius, vertices);
    }

private:

    glm::vec3 a;
    glm::vec3 b;
    glm::vec3 c;

    static void appendVertex(std::vector<float>& vertices, const glm::vec3& position, const glm::vec3& normal) {

        vertices.insert(vertices.end(), {
            position.x, position.y, position.z, normal.x, normal.y, normal.z
        });
    }
};