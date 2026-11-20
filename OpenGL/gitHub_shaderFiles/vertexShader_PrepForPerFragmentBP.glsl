#version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;

out vec4 normal;
out vec3 lightDir;
out vec3 halfV;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

uniform vec4 lightPosWorld;

void main(void)
{
    // Transform the vertex and light into camera space.
    vec4 pos = viewMatrix * modelMatrix * vec4(in_Position, 1.0);
    vec4 lightPos = viewMatrix * lightPosWorld;

    normal = normalMatrix * vec4(in_Normal, 0.0);

    vec3 v = normalize(-pos.xyz);
    lightDir = normalize(lightPos.xyz - pos.xyz);
    halfV = normalize(v + lightDir);

    gl_Position = projMatrix * pos;
    // for normal
    // gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);

}