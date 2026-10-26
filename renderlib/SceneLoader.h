#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "ISceneLoader.h"
#include "scene.h"

class SceneLoader : public ISceneLoader {
private:
    Scene& m_targetScene;

    int m_imageWidth;
    int m_imageHeight;

    std::vector<std::shared_ptr<PerspectiveCamera>> m_cameras;
    std::vector<std::shared_ptr<Shape>> m_shapes;

    std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;

    static vec3 toVec3(const ISceneLoader::vec& value) {
        return vec3(value.x, value.y, value.z);
    }

public:
  // The caller provides the scene to be filled
  // SceneLoader(Scene &sceneToPopulate)
  //     : m_targetScene(sceneToPopulate), numShaders(0), numTextures(0) {}

  SceneLoader(Scene &sceneToPopulate, int imageWidth, int imageHeight)
      : m_targetScene(sceneToPopulate),
        m_imageHeight(imageHeight),
        m_imageWidth(imageWidth) {}
  
  void reserveCameras(size_t count) override {
    // makes sure the vector has enough space
    m_cameras.reserve(count);
  }
  void reserveLights(size_t /*count*/) override {
    // m_targetScene.lights.reserve(count);
  }
  void reserveShapes(size_t count) override {
    m_shapes.reserve(count);
  }
  void reserveShaders(size_t count) override { numShaders = count; }
  void reserveTextures(size_t count) override { numTextures = count; }

  void setSceneParams(const SceneParams &sparams) override {
    std::cout << "Setting scene params." << std::endl;

    m_targetScene.setBackground(toVec3(sparams.bgColor));
    
  }

  void addCamera(const std::string &name, const std::string &type,
                 const ISceneLoader::vec &pos, const ISceneLoader::vec &viewDir,
                 float focalLength, float imagePlaneWidth) override {
    std::cout << "Creating camera: " << name << ", type:" << type << std::endl;
    auto camToAdd = std::make_shared<PerspectiveCamera>(
      toVec3(viewDir), toVec3(pos), 
      m_imageWidth, m_imageHeight, 
      focalLength, imagePlaneWidth);
    

    m_cameras.push_back(camToAdd);  
    m_targetScene.addCam(*camToAdd);
    
  }
  void addPointLight(const ISceneLoader::vec &pos,
                     const ISceneLoader::vec &intensity) override {

    std::cout << "Creating point light." << std::endl;
    PointLight light;

    light.position = toVec3(pos);
    light.intesity = toVec3(intensity);


    m_targetScene.addLight(light);
  }

  void addAreaLight(const ISceneLoader::vec& /*pos*/,
                    const ISceneLoader::vec& /*color*/,
                    const ISceneLoader::vec& /*normal*/, float /*width*/,
                    float /*length*/) override {
    std::cout << "Creating area light." << std::endl;

  }

  void addShader(const ISceneLoader::ShaderDesc &shaderDesc) override {
    std::cout << "Creating shader: type=" << shaderDesc.type << std::endl;

    std::shared_ptr<Shader> shader;

    if(shaderDesc.type == "Lambertian") {

      shader = std::make_shared<Lambertian>(
        toVec3(shaderDesc.diffuse.data)
      );

    }
    else if (shaderDesc.type == "BlinnPhong") {

      shader = std::make_shared<BlinnPhong>(
        toVec3(shaderDesc.diffuse.data),
        toVec3(shaderDesc.specular.data),
        shaderDesc.phongExp
      );

    }
    else if (shaderDesc.type == "Mirror") {

      shader = std::make_shared<Mirror>();

    }
    else if (shaderDesc.type == "Diffuse") {

      shader = std::make_shared<Diffuse>(
        toVec3(shaderDesc.diffuse.data)
      );

    }
    else if (shaderDesc.type == "Normal") {

      shader = std::make_shared<Normal>();

    }
    else {
      throw std::runtime_error(
        "unsupported shader type: " + shaderDesc.type
      );
    }

    m_shaders.emplace(shaderDesc.name, shader);
  }

  void addShape(const ISceneLoader::ShapeDesc &shapeDesc) override {
    std::cout << "Creating shape: type=" << shapeDesc.type << std::endl;

      auto enteredShader = m_shaders.find(shapeDesc.shaderNameReference);

      std::shared_ptr<Shape> shape;

      if (shapeDesc.type == "sphere") {

        shape = std::make_shared<Sphere>(
          toVec3(shapeDesc.center),
          shapeDesc.radius
        );

      }
      else if (shapeDesc.type == "triangle") {

        shape = std::make_shared<Triangle>(
          toVec3(shapeDesc.v0),
          toVec3(shapeDesc.v1),
          toVec3(shapeDesc.v2)
        );
      }
      else {
        throw std::runtime_error(
          "unsupported shape type: " + shapeDesc.type
        );
    }


    m_shapes.push_back(shape);

    // Connect the shape to its named shader
    m_targetScene.addObject(*shape, *enteredShader->second);
  }

  void addTexture(const std::string &/*type*/, const std::string &name,
                  const std::string &/*sourceFile*/) override {
    // std::cout << "Creating texture: type=" << type << ", name=" << name
    //           << ", sourceFile=" << sourceFile << std::endl;
  }

public:
  int numShaders;
  int numTextures;
};
