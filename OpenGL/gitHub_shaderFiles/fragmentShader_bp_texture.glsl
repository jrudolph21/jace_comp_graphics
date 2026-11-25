

#version 330 core

layout(location = 0) out vec4 fragmentColor;

in vec4 normal;
in vec3 lightDir;
in vec3 halfV;
in vec2 tCoord;

uniform vec3 lightIntensity;
uniform sampler2D textureUnit;

uniform vec3 Ia;
uniform vec3 ka, kd, ks;
uniform float phongExp;

void main(void)
{
    vec3 surfaceNormal = normalize(normal.xyz);
    vec3 l = normalize(lightDir);
    vec3 texel = texture(textureUnit, tCoord).rgb;

    vec3 color = ka * Ia * texel;

    float nDotLight = dot(surfaceNormal, l);


    if (nDotLight > 0.0)
    {
        // diffuse
        vec3 diffuse = kd * texel * nDotLight;
        vec3 specular = vec3(0.0);

        // Halfway vector 
        vec3 halfway = halfV;

        if (length(halfway) > 1e-8)
        {
            vec3 h = normalize(halfway);

            // specular
            float nDotH = max(0.0, dot(surfaceNormal, h));
            specular = ks * pow(nDotH, phongExp);
        }

        color += (diffuse + specular) * lightIntensity;
    }

    fragmentColor = vec4(color, 1.0);
}