#pragma once

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ISceneLoader.h"

struct OpenGLSceneData {
    struct CameraData {
        glm::vec3 position, direction;
        float focalLength, planeWidth;
    };
    struct LightData { glm::vec3 position, intensity; };
    std::vector<CameraData> cameras;
    std::vector<LightData> lights;
    std::vector<ISceneLoader::ShaderDesc> shaders;
    std::vector<ISceneLoader::ShapeDesc> shapes;
    glm::vec3 background{0.02f, 0.02f, 0.025f};
};

class SceneLoader : public ISceneLoader {
public:
    explicit SceneLoader(OpenGLSceneData& scene) : m_targetScene(scene) {}

    static glm::vec3 convert(const ISceneLoader::vec& v) {
        if (!std::isfinite(v.x) || !std::isfinite(v.y) || !std::isfinite(v.z))
            throw std::runtime_error("Non-finite scene vector.");
        return glm::vec3(v.x, v.y, v.z);
    }
    void reserveCameras(size_t n) override { m_targetScene.cameras.reserve(n); }
    void reserveLights(size_t n) override { m_targetScene.lights.reserve(n); }
    void reserveShaders(size_t n) override { m_targetScene.shaders.reserve(n); }
    void reserveShapes(size_t n) override { m_targetScene.shapes.reserve(n); }
    void reserveTextures(size_t) override {}
    void setSceneParams(const SceneParams& p) override { m_targetScene.background = convert(p.bgColor); }
    void addCamera(const std::string&, const std::string& type,
                   const ISceneLoader::vec& pos, const ISceneLoader::vec& dir,
                   float focal, float width) override {
        if (type != "perspective") throw std::runtime_error("Only perspective cameras are supported.");
        m_targetScene.cameras.push_back({convert(pos), convert(dir), focal, width});
    }
    void addPointLight(const ISceneLoader::vec& pos, const ISceneLoader::vec& intensity) override {
        m_targetScene.lights.push_back({convert(pos), convert(intensity)});
    }
    void addAreaLight(const ISceneLoader::vec&, const ISceneLoader::vec&,
                      const ISceneLoader::vec&, float, float) override {
        throw std::runtime_error("Area lights are not supported by this GLSL shader.");
    }
    void addShader(const ShaderDesc& d) override { m_targetScene.shaders.push_back(d); }
    void addShape(const ShapeDesc& d) override {
        ShapeDesc shape{};
        shape.type = d.type;
        shape.name = d.name;
        shape.shaderNameReference = d.shaderNameReference;
        shape.hasXForm = d.hasXForm;
        shape.localXForm = d.localXForm;
        if (d.type == "sphere") {
            shape.center = d.center;
            shape.radius = d.radius;
        } else if (d.type == "triangle") {
            shape.v0 = d.v0;
            shape.v1 = d.v1;
            shape.v2 = d.v2;
        } else {
            throw std::runtime_error("Unsupported OpenGL shape: " + d.type);
        }
        m_targetScene.shapes.push_back(shape);
    }
    void addTexture(const std::string&, const std::string&, const std::string&) override {
        throw std::runtime_error("Textures are not supported by this renderer.");
    }
private:
    OpenGLSceneData& m_targetScene;
};
