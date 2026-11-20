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

        void moveUp(float distance) {
            position += distance * V;
        }

        void rotateView(float yawChange, float pitchChange) {

            yaw += yawChange;
            pitch = glm::clamp(pitch + pitchChange, -89.0f, 89.0f);

            const float yawRadians = glm::radians(yaw);
            const float pitchRadians = glm::radians(pitch);

            glm::vec3 direction;
            direction.x = std::cos(yawRadians) * std::cos(pitchRadians);
            direction.y = std::sin(pitchRadians);
            direction.z = std::sin(yawRadians) * std::cos(pitchRadians);

            W = -glm::normalize(direction);
            U = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), W));
            V = glm::cross(W, U);
            
        }

    private:
        glm::vec3 position;
        glm::vec3 U, V, W;

        float planeWidth;
        float focalLength;
        float nearPlane;
        float farPlane;

        float yaw = -90.0f;
        float pitch = 0.0f;
};