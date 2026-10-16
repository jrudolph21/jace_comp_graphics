#include "shader.h"
#include "scene.h"
#include <algorithm>
#include <limits>

Lambertian::Lambertian() : kd(vec3(1.0, 1.0, 1.0)) {}

vec3 Lambertian::rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights, const std::vector<Shape*>& shapeObjs, int depth) const {

    if(depth <= 0) {
        return vec3(0, 0, 0);
    }

    vec3 normal = hit.normal;

    vec3 color(0.0, 0.0, 0.0);

    for(const PointLight& light : vecLights) {

        //Unit vector point from shap intersection to light
        vec3 l = unit_vec(light.position-hit.intersectionPoint);

        vec3 shadowOrigin = hit.intersectionPoint + 0.0001f * normal;

        vec3 toLight = light.position - shadowOrigin;
        ray shadowRay(shadowOrigin, unit_vec(toLight));

        float tmax = static_cast<float>(toLight.length()) - 0.0001f;

        double nDotLight = std::max(0.0, dot(normal, l));

        bool inShadow = false;

        for(Shape* shape : shapeObjs) {

            HitStruct shadowStruct;

            if (shape->intersect(shadowRay, 0.0001f, tmax, shadowStruct)) {
                inShadow = true;
                break;
            }
        }
        if (!inShadow) {
            color += kd * nDotLight * light.intesity;
        }
        
    }

    return color;
}

BlinnPhong::BlinnPhong() : view(vec3(0.0, 0.0, 0.0)), h(vec3(0.0, 0.0, 0.0)), kd(vec3(1.0, 1.0, 1.0)), ks(vec3(5.0, 5.0, 5.0)), p(50.0) {}

void BlinnPhong::setP(double p_value) {
    p = p_value;
}

void BlinnPhong::setKd(const vec3& value) {
    kd = value;
}


vec3 BlinnPhong::rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights, const std::vector<Shape*>& shapeObjs, int depth) const {
    if (depth <= 0) {
        return vec3(0, 0, 0);
    }

    vec3 normal = unit_vec(hit.normal);

    // Direction from intersection toward the viewer
    vec3 v = unit_vec(-1.0 * hit.viewDir);

    vec3 color(0.0, 0.0, 0.0);

    for (const PointLight& light : vecLights) {

        // Direction from intersection to light
        vec3 l = unit_vec(
            light.position - hit.intersectionPoint
        );

        double nDotLight = dot(normal, l);

        // Light behind surface contributes nothing
        if (nDotLight <= 0.0) {
            continue;
        }

        // Shadow ray
        vec3 shadowOrigin =
            hit.intersectionPoint + 0.0001f * normal;

        vec3 toLight = light.position - shadowOrigin;

        float tmax =
            static_cast<float>(toLight.length()) - 0.0001f;

        ray shadowRay(shadowOrigin, unit_vec(toLight));

        bool inShadow = false;

        for (Shape* shape : shapeObjs) {
            HitStruct shadowHit;

            if (shape->intersect(
                shadowRay, 0.0001f, tmax, shadowHit
            )) {
                inShadow = true;
                break;
            }
        }

        if (inShadow) {
            continue;
        }

        // Halfway vector
        vec3 halfway = l + v;

        if (halfway.length() < 1e-8) {
            continue;
        }

        vec3 h = unit_vec(halfway);

        // Lambertian diffuse
        vec3 diffuse = kd * nDotLight;

        // Blinn-Phong specular
        double nDotH = std::max(0.0, dot(normal, h));

        vec3 specular = ks * std::pow(nDotH, p);

        // Sum contributions from all visible lights
        color += (diffuse + specular) * light.intesity;
    }

    return color;
}


vec3 Mirror::rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights, const std::vector<Shape*>& shapeObjs, int depth) const {
    if (depth <= 0 || hit.scene == nullptr) {
        return vec3(0, 0, 0);
    }

    vec3 normal = unit_vec(hit.normal);

    // Point back toward the source of the incoming ray.
    vec3 v = unit_vec(-1.0 * hit.viewDir);

    if (dot(normal, v) < 0.0) {
        normal = -1.0 * normal;
    }

    // Reflected direction.
    vec3 l = unit_vec(
        2.0 * dot(normal, v) * normal - v
    );


    vec3 shadowOrigin =
        hit.intersectionPoint + 0.0001 * normal;

    ray shadowRay(shadowOrigin, l);

    float tmax = std::numeric_limits<float>::infinity();

    // Reflections that miss all objects show the background.
    vec3 color = hit.scene->getBackground();

    HitStruct closestHit;
    const Scene::SceneObject* closestObject = nullptr;

    for (const Scene::SceneObject& object :
         hit.scene->getObjects()) {

        HitStruct shadowHit;

        if (object.shape->intersect(
                shadowRay, 0.0001f, tmax, shadowHit
            )) {

            shadowHit.viewDir = shadowRay.getDirection();
            shadowHit.scene = hit.scene;

            closestHit = shadowHit;
            closestObject = &object;
        }
    }

    if (closestObject != nullptr) {
        vec3 shading = closestObject->shader->rayColor(
            closestHit,
            vecLights,
            shapeObjs,
            depth - 1
        );

        const vec3& base = closestObject->color;

        color = vec3(
            base.x() * shading.x(),
            base.y() * shading.y(),
            base.z() * shading.z()
        );
    }

    return color;
}

Diffuse::Diffuse() : kd(vec3(1.0, 1.0, 1.0)) {}

vec3 Diffuse::rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights, const std::vector<Shape*>& shapeObjs, int depth) const {

     if(depth <= 0) {
        return vec3(0, 0, 0);
    }

    vec3 normal = hit.normal;

    vec3 color(0.0, 0.0, 0.0);

    for(const PointLight& light : vecLights) {

        //Unit vector point from shap intersection to light
        vec3 l = unit_vec(light.position-hit.intersectionPoint);

        vec3 shadowOrigin = hit.intersectionPoint + 0.0001f * normal;

        vec3 toLight = light.position - shadowOrigin;
        ray shadowRay(shadowOrigin, unit_vec(toLight));

        float tmax = static_cast<float>(toLight.length()) - 0.0001f;

        double nDotLight = std::max(0.0, dot(normal, l));

        bool inShadow = false;

        for(Shape* shape : shapeObjs) {

            HitStruct shadowStruct;

            if (shape->intersect(shadowRay, 0.0001f, tmax, shadowStruct)) {
                inShadow = true;
                break;
            }
        }
        if (!inShadow) {
            color += kd * nDotLight * light.intesity;
        }
        
    }

    if (depth > 1) {

        vec3 bounce = unit_vec(normal + random_in_unit_sphere());

        vec3 bounceOrigin = hit.intersectionPoint * normal;

        ray bounceRay(bounceOrigin, bounce);

        float infinity = std::numeric_limits<float>::infinity();
        HitStruct closestHit;
        bool hitSomething = false;

        for (Shape* shape : shapeObjs) {
            HitStruct candidateHit;

            // intersect updates closestT when it finds a closer hit.
            if (shape->intersect(
                    bounceRay, 0.0001f, infinity, candidateHit)) {

                closestHit = candidateHit;
                hitSomething = true;
            }
        }

        if (hitSomething && closestHit.shader != nullptr) {
            vec3 bouncedColor = closestHit.shader->rayColor(
                closestHit,
                vecLights,
                shapeObjs,
                depth - 1
            );

            color += kd * bouncedColor;
        }

        // A missed bounce contributes black here.
        // You could instead add kd * yourBackgroundColor.
    }

    return color;

}

vec3 Diffuse::random_in_unit_sphere() const {

    while (true) {
        vec3 ranDir = getRandomVec3(-1.0, 1.0);
        if (ranDir.length() >= 1) continue;

        return unit_vec(ranDir);
    }
}


vec3 Diffuse::getRandomVec3(double min, double max) const {
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;

    return vec3(
        distribution(generator),
        distribution(generator),
        distribution(generator)
    );

}

vec3 Normal::rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights, const std::vector<Shape*>& shapeObjs, int depth) const {

    if(depth <= 0) {
        return vec3(0, 0, 0);
    }
    vec3 normal = unit_vec(hit.normal);

    return 0.5 * (normal + vec3(1, 1, 1));
}
