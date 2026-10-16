#include "rayTracer.h"
#include <limits>
#include <random>

float randomOffset() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

void rayTracer::render(
    const Scene& scene,
    frameBuffer& fb,
    int cameraIndex, 
    int depth
) {
    Camera& camera = scene.getCam(cameraIndex);

    const auto& objects = scene.getObjects();
    const auto& lights = scene.getLights();
    const auto& shapes = scene.getShapes();

    const int rpp_NSquare = 10;
    

    for (int x = 0; x < fb.getWidth(); ++x) {
        for (int y = 0; y < fb.getHeight(); ++y) {

            vec3 pixelColor(0.0,0.0,0.0);

            for(int p = 0; p < rpp_NSquare; ++p) {
                for (int q = 0; q < rpp_NSquare; ++q) {

                    double xOffset = (p + randomOffset()) / rpp_NSquare;

                    double yOffset = (q + randomOffset()) / rpp_NSquare;

                    ray r = camera.generateRay(x+xOffset - 0.5, y+yOffset - 0.5);

                    
                    const float tmin = 1.0f;
                    float tmax = std::numeric_limits<float>::infinity();

                    HitStruct closestHit;
                    closestHit.scene = &scene;
                    const Scene::SceneObject* closestObject = nullptr;

                    vec3 sampleColor = scene.getBackground();

                    // Find the closest intersection.
                    // intersect() must lower tmax whenever it accepts a hit.
                    for (const Scene::SceneObject& object : objects) {
                        HitStruct candidate;

                        if (object.shape->intersect(r, tmin, tmax, candidate)) {
                            candidate.viewDir = r.getDirection();

                            closestHit = candidate;
                            closestObject = &object;
                        }
                    }

                    if (closestObject != nullptr) {
                        closestHit.scene = &scene;

                        vec3 shading = closestObject->shader->rayColor(
                            closestHit, lights, shapes, depth
                        );

                        const vec3& base = closestObject->color;

                        sampleColor = vec3(
                            base.x() * shading.x(),
                            base.y() * shading.y(),
                            base.z() * shading.z()
                        );
                    }

                    pixelColor += sampleColor;

                }
            }

            pixelColor = pixelColor/(rpp_NSquare*rpp_NSquare);
            fb.set(x, y, pixelColor);
        }
    }
}

