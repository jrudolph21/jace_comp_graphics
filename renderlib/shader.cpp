#include "shader.h"
#include <algorithm>

Lambertian::Lambertian() : kd(vec3(1.0, 1.0, 1.0)) {}

vec3 Lambertian::rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights) {

    vec3 normal = hit.normal;

    vec3 color(0.0, 0.0, 0.0);

    for(const PointLight& light : vecLights) {

        vec3 l = unit_vec(light.position-hit.intersectionPoint);

        double nDotLight = std::max(0.0, dot(normal, l));

        color += kd * nDotLight * light.intesity;
    }

    return color;
}

BlinnPhong::BlinnPhong() : view(vec3(0.0, 0.0, 0.0)), h(vec3(0.0, 0.0, 0.0)), kd(vec3(1.0, 1.0, 1.0)), ks(vec3(1.0, 1.0, 1.0)), p(50.0) {}

vec3 BlinnPhong::rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights) {


    vec3 normal = hit.normal;

    vec3 v = unit_vec(-1*(hit.viewDir));

    vec3 color(0.0, 0.0, 0.0);

    for(const PointLight& light : vecLights) {

        vec3 l = unit_vec(light.position-hit.intersectionPoint);

        h = unit_vec(l+v);

        vec3 diffuse = kd * std::max(0.0, dot(normal, l));

        //double nDoth = std::max(0.0, dot(normal, h));

        vec3 specular = ks * std::pow(std::max(0.0, dot(normal, h)), p);

        color += diffuse + specular;

    }

    return color;


}

vec3 Normal::rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights) {

    vec3 normal = unit_vec(hit.normal);

    return 0.5 * (normal + vec3(1, 1, 1));
}
