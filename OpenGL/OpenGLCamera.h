#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class OpenGLCamera {
    
    public:
        OpenGLCamera(glm::vec3 eye, glm::vec3 viewDirection, glm::vec3 worldUp,
                    float planeWidth, float focalLength,
                    float nearPlane, float farPlane) 
                    : position(eye), planeWidth(planeWidth), focalLength(focalLength), nearPlane(nearPlane), farPlane(farPlane) {
                        
            W = glm::normalize(-viewDirection);
            U = glm::normalize(glm::cross(worldUp, W));
            V = glm::cross(W, U);
        }

        glm::mat4 getViewMatrix() const {
            return glm::lookAt(position, position - W, V);
        }

        glm::mat4 getProjectionMatrix(int width, int height) const {
            const float aspect = static_cast<float>(width) / height;
            const float planeHeight = planeWidth / aspect;
            const float fovY = 2.0f * std::atan((planeHeight / 2.0f) / focalLength);

            return glm::perspective(fovY, aspect, nearPlane, farPlane);
        }

        void moveForward(float distance) {
            position -= distance * W;
        }

        void moveRight(float distance) {
            position += distance * U;
        }

    private:
        glm::vec3 position;
        glm::vec3 U, V, W;

        float planeWidth;
        float focalLength;
        float nearPlane;
        float farPlane;
};