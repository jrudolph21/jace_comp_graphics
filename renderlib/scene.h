#pragma once

#include "camera.h"
#include "shape.h"
#include "shader.h"

#include <vector>

class Scene {

    public:

        Scene() = default;

        void addCam(Camera& camera);
        void addLight(const PointLight& light);

        void addObject(Shape& shape, Shader& shader);

        void setBackground(const vec3& color);

        struct SceneObject {
            Shape* shape; 
            Shader* shader;
        };

        const std::vector<PointLight>& getLights() const;
        const std::vector<SceneObject>& getObjects() const;
        const std::vector<Shape*>& getShapes() const;
        const vec3& getBackground() const;

        Camera& getCam(int index = 0) const;



    private: 

        std::vector<PointLight> lights;
        std::vector<Camera*> cameras;
        std::vector<SceneObject> objects;

        std::vector<Shape*> shapes;

        vec3 background = vec3(0,0,0);

    };
        