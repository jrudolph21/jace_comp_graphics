#include "scene.h"
#include <algorithm>

void Scene::addCam(Camera& camera) {
    cameras.push_back(&camera);
}

void Scene::addLight(const PointLight& light) {
    lights.push_back(light);
}

void Scene::addObject(Shape& shape, Shader& shader) {
    objects.push_back({&shape, &shader});
    shapes.push_back(&shape);
}

void Scene::setBackground(const vec3& color) {
    background = color;
}

Camera& Scene::getCam(int index) const {
    return *cameras[index];
}

const std::vector<PointLight>& Scene::getLights() const {
    return lights;
}

const std::vector<Scene::SceneObject>& Scene::getObjects() const {
    return objects;
}

const std::vector<Shape*>& Scene::getShapes() const {
    return shapes;
}

const vec3& Scene::getBackground() const {
    return background;
}
