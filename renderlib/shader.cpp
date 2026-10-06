#include "shader.h"
#include <algorithm>

Lambertian::Lambertian() : kd(vec3(1.0, 1.0, 1.0)) {}

vec3 Lambertian::rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights, const std::vector<Shape*>& shapeObjs) {

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

BlinnPhong::BlinnPhong() : view(vec3(0.0, 0.0, 0.0)), h(vec3(0.0, 0.0, 0.0)), kd(vec3(1.0, 1.0, 1.0)), ks(vec3(1.0, 1.0, 1.0)), p(50.0) {}

vec3 BlinnPhong::rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights, const std::vector<Shape*>& shapeObjs) {


    vec3 normal = hit.normal;

    // reverse the incoming view ray to point back at camera, then normalize
    vec3 v = unit_vec(-1*(hit.viewDir));

    vec3 color(0.0, 0.0, 0.0);

    for(const PointLight& light : vecLights) {

        //Unit vector pointing from shape intersection to light
        vec3 l = unit_vec(light.position-hit.intersectionPoint);

        double nDotLight = std::max(0.0, dot(normal, l));

        vec3 shadowOrigin = hit.intersectionPoint + 0.0001f * normal;

        vec3 toLight = light.position - shadowOrigin;
        ray shadowRay(shadowOrigin, unit_vec(toLight));

        float tmax = static_cast<float>(toLight.length()) - 0.0001f;

        bool inShadow = false;

        for(Shape* shape : shapeObjs) {
            HitStruct shadowHit;

            if(shape->intersect(shadowRay, 0.0001f, tmax, shadowHit)) {
                inShadow = true;
                break;
            }
        }

        if(inShadow) {
            continue;
        }
        //Halfway vector betwen light and view
        h = unit_vec(l+v);

        vec3 diffuse = kd * nDotLight;

        vec3 specular = ks * std::pow(std::max(0.0, dot(normal, h)), p);

        color += (diffuse + specular) * light.intesity;

    }

    return color;


}

vec3 Normal::rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights, const std::vector<Shape*>& shapeObjs) {

    vec3 normal = unit_vec(hit.normal);

    return 0.5 * (normal + vec3(1, 1, 1));
}
