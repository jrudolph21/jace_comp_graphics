#pragma once
#include "HitStruct.h"
#include "PointLight.h"
#include <vector>


class Shader {

    public:

        virtual ~Shader() = default;

        virtual vec3 rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights) = 0;
};


class Lambertian : public Shader {

    public:

        Lambertian();

        vec3 rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights) override;

    private:

        vec3 kd; // reflectance of surface

};

class BlinnPhong : public Shader {

    public:

        BlinnPhong();

        vec3 rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights) override;

        void setP();
        
    private:

        vec3 view;

        vec3 h;

        vec3 kd, ks;
        double p;

};

class Normal : public Shader {

    public: 

        Normal() {}

        vec3 rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights) override;


};

