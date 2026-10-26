#pragma once
#include "shape.h"
#include "HitStruct.h"
#include "PointLight.h"
#include <vector>
#include <limits>
#include <random>


class Shader {

    public:

        virtual ~Shader() = default;

        virtual vec3 rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights, const std::vector<Shape*>& shapeObjs, int depth) const = 0;
};


class Lambertian : public Shader {

    public:

        Lambertian();
        Lambertian(const vec3 &color);

        vec3 rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights, const std::vector<Shape*>& shapeObjs, int depth) const override;

    private:

        vec3 kd; // reflectance of surface

};

class BlinnPhong : public Shader {

    public:

        BlinnPhong();
        BlinnPhong(const vec3 &color, const vec3 &specular, double phong);

        vec3 rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights, const std::vector<Shape*>& shapeObjs, int depth) const override;
        void setP(double p_value);
        void setKd(const vec3& value);
        
    private:

        vec3 view;

        vec3 h;

        vec3 kd, ks;
        double p;

};

class Mirror : public Shader {

    public:

        vec3 rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights, const std::vector<Shape*>& shapeObjs, int depth) const override;

    private:

        vec3 view;

        vec3 h;

        vec3 kd, ks;
        double p;

};

class Diffuse : public Shader {

    public:

        Diffuse();
        Diffuse(const vec3 &color);

        vec3 rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights, const std::vector<Shape*>& shapeObjs, int depth) const override;

    private:

        vec3 random_in_unit_sphere() const;

        vec3 getRandomVec3(double min, double max) const;

        vec3 kd; // reflectance of surface

};


class Normal : public Shader {

    public: 

        Normal() {}

        vec3 rayColor(const HitStruct &hit, const std::vector<PointLight>& vecLights, const std::vector<Shape*>& shapeObjs, int depth) const override;


};

