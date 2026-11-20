#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <cmath>
#include <nlohmann/json.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLSL.h"
#include "OpenGLCamera.h"
#include "SceneLoader.h"
#include "SceneParser_JSON.h"
#include <memory>
#include "OpenGLTriangle.h"

std::vector<float> createSphere(int depth, float radius)
{
    const glm::vec3 top(0.0f, 1.0f, 0.0f);
    const glm::vec3 bottom(0.0f, -1.0f, 0.0f);
    const glm::vec3 left(-1.0f, 0.0f, 0.0f);
    const glm::vec3 right(1.0f, 0.0f, 0.0f);
    const glm::vec3 front(0.0f, 0.0f, 1.0f);
    const glm::vec3 back(0.0f, 0.0f, -1.0f);

    const OpenGLTriangle faces[] = {
        OpenGLTriangle(top, front, right),
        OpenGLTriangle(top, right, back),
        OpenGLTriangle(top, back, left),
        OpenGLTriangle(top, left, front),

        OpenGLTriangle(bottom, right, front),
        OpenGLTriangle(bottom, back, right),
        OpenGLTriangle(bottom, left, back),
        OpenGLTriangle(bottom, front, left)
    };

    std::vector<float> vertices;

    for (const OpenGLTriangle& face : faces) {
        face.subdivide(depth, radius, vertices);
    }

    return vertices;
}


using json = nlohmann::json;
struct Material {
    glm::vec3 kd, ka, ks;
    float exponent;
};
struct Orbit {
    glm::vec3 pivot, axis;
    float angle, speed;
};
struct Sphere {
    glm::vec3 center;
    float radius;
    Material material;
    int orbit = -1;
};
struct TriangleBatch {
    Material material;
    int orbit = -1;
    std::vector<float> vertices; 
    GLuint vao = 0, vbo = 0;
    GLsizei vertexCount = 0;
};
struct SceneData {
    glm::vec3 eye, direction, lightPosition, lightIntensity;
    glm::vec3 background{0.02f, 0.02f, 0.025f}, ambient{1.0f};
    float planeWidth, focalLength, nearPlane, farPlane;
    int depth = 4;
    std::vector<Orbit> orbits;
    std::vector<Sphere> spheres;
    std::vector<TriangleBatch> triangles;
};
float number(const json& j) {
    return j.get<float>();
}
glm::vec3 vector3(const json& j) {
    if (j.is_array()) {
        return glm::vec3(number(j[0]), number(j[1]), number(j[2]));
    }
    glm::vec3 v;
    std::istringstream in(j.get<std::string>());
    in >> v.x >> v.y >> v.z;
    return v;
}
std::string field(const json& j, const char* name, const char* alias,
                  const std::string& fallback = "") {
    return j.value(name, j.value(alias, fallback));
}

SceneData loadScene(const std::string& filename) {
    std::ifstream input(filename);
    json root;
    input >> root;
    const json* sceneJson = &root;
    if (root.contains("scene")) {
        sceneJson = &root.at("scene");
    }
    const json& j = *sceneJson;
   
    OpenGLSceneData loadedScene;
    auto loader = std::make_shared<SceneLoader>(loadedScene);
    SceneParser_JSON parser(loader);

    parser.parseFileData(filename);
    SceneData out;
    out.background = loadedScene.background;
    if (j.contains("sceneParameters")) {
        const auto& p = j.at("sceneParameters");

        if (p.contains("ambientIntensity")) {
            out.ambient = vector3(p.at("ambientIntensity"));
        }
        out.depth = p.value("sphereDepth", 4);
    }
    const auto& c = j.at("camera").at(0);
    out.eye = loadedScene.cameras.front().position;
    out.direction = loadedScene.cameras.front().direction;
    out.planeWidth = loadedScene.cameras.front().planeWidth;
    out.focalLength = loadedScene.cameras.front().focalLength;
    out.nearPlane = number(c.value("nearPlane", json(0.1)));
    out.farPlane = number(c.value("farPlane", json(100.0)));
    out.lightPosition = loadedScene.lights.front().position;
    out.lightIntensity = loadedScene.lights.front().intensity;

    std::unordered_map<std::string, glm::vec3> ambientByName;
    for (const auto& m : j.at("shader")) {
        if (m.contains("ambient")) {
            ambientByName[field(m, "name", "_name")] = vector3(m.at("ambient"));
        }
    }
    std::unordered_map<std::string, Material> materials;
    for (const auto& m : loadedScene.shaders) {
        const auto& type = m.type;
        Material material;
        material.kd = SceneLoader::convert(m.diffuse.data);
        if (ambientByName.count(m.name)) {
            material.ka = ambientByName.at(m.name);
        } else {
            material.ka = 0.05f * material.kd;
        }

        if (type == "BlinnPhong") {
            material.ks = SceneLoader::convert(m.specular.data);
            material.exponent = m.phongExp;
        } else {
            material.ks = glm::vec3(0);
            material.exponent = 1.0f;
        }
        const auto& name = m.name;
        materials[name] = material;
    }
    std::unordered_map<std::string, int> orbitIndices;
    for (const auto& o : j.value("orbit", json::array())) {
        Orbit orbit;
        orbit.pivot = vector3(o.value("pivot", json::array({0,0,0})));
        orbit.axis = vector3(o.at("axis"));
        orbit.axis = glm::normalize(orbit.axis);
        orbit.angle = glm::radians(number(o.value("angleDegrees", json(0))));
        orbit.speed = glm::radians(number(o.value("speedDegrees", json(0))));
        const auto name = field(o, "name", "_name");
        orbitIndices[name] = static_cast<int>(out.orbits.size());
        out.orbits.push_back(orbit);
    }

    const json bindings = j.value("animation", json::object()).value("shapeOrbits", json::object());
    std::map<std::pair<std::string, int>, size_t> batchIndices;
    size_t triangleCount = 0;
    for (const auto& shape : loadedScene.shapes) {
        const auto& materialName = shape.shaderNameReference;
        int orbitIndex = -1;
        if (bindings.contains(shape.name)) {
            const auto name = bindings.at(shape.name).get<std::string>();
            orbitIndex = orbitIndices.at(name);
        }
        if (shape.type == "sphere") {
            Sphere sphere;
            sphere.center = SceneLoader::convert(shape.center);
            sphere.radius = shape.radius;
            sphere.material = materials.at(materialName);
            sphere.orbit = orbitIndex;
            out.spheres.push_back(sphere);
        } else if (shape.type == "triangle") {
            const glm::vec3 v0 = SceneLoader::convert(shape.v0);
            const glm::vec3 v1 = SceneLoader::convert(shape.v1);
            const glm::vec3 v2 = SceneLoader::convert(shape.v2);
            const glm::dvec3 crossProduct = glm::cross(glm::dvec3(v1) - glm::dvec3(v0),
                                                      glm::dvec3(v2) - glm::dvec3(v0));
            const double areaSquared = glm::dot(crossProduct, crossProduct);
            const glm::vec3 normal(crossProduct / std::sqrt(areaSquared));
            const auto key = std::make_pair(materialName, orbitIndex);
            if (!batchIndices.count(key)) {
                batchIndices.emplace(key, out.triangles.size());
                TriangleBatch batch;
                batch.material = materials.at(materialName);
                batch.orbit = orbitIndex;
                out.triangles.push_back(batch);
            }
            auto& vertices = out.triangles[batchIndices.at(key)].vertices;
            for (const glm::vec3& v : {v0, v1, v2}) {
                vertices.insert(vertices.end(), {v.x, v.y, v.z, normal.x, normal.y, normal.z});
            }
            ++triangleCount;
        }
    }
    std::cout << "Loaded " << out.spheres.size() << " spheres and " << triangleCount
              << " triangles in " << out.triangles.size() << " triangle batches.\n";
    return out;
}

int main(int argc, char* argv[])
{
    std::string filename = "silverLevelScene.json";
    if (argc == 3 && std::string(argv[1]) == "--input") {
        filename = argv[2];
    } else if (argc == 2) {
        filename = argv[1];
    }
    SceneData scene = loadScene(filename);

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(1000, 1000, "Silver Level", NULL, NULL);

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    const GLubyte* renderer = glGetString (GL_RENDERER);
    const GLubyte* version = glGetString (GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(scene.background.x, scene.background.y, scene.background.z, 1.0f);
    std::vector<float> sphereOfTriangles;
    if (!scene.spheres.empty()) {
        sphereOfTriangles = createSphere(scene.depth, 1.0f);
    }

    const GLsizei vertexCount = static_cast<GLsizei>(sphereOfTriangles.size() / 6);

    GLuint sphereVBO;
    glGenBuffers(1, &sphereVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);

    GLuint sphereVAO;
    glGenVertexArrays(1, &sphereVAO);
    glBindVertexArray(sphereVAO);

    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sphereOfTriangles.size() * sizeof(float)), sphereOfTriangles.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    sphereOfTriangles.clear();

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    for (auto& batch : scene.triangles) {
        batch.vertexCount = static_cast<GLsizei>(batch.vertices.size() / 6);
        glGenVertexArrays(1, &batch.vao);
        glGenBuffers(1, &batch.vbo);
        glBindVertexArray(batch.vao);
        glBindBuffer(GL_ARRAY_BUFFER, batch.vbo);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(batch.vertices.size() * sizeof(float)),
                     batch.vertices.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                              reinterpret_cast<void*>(3 * sizeof(float)));
        std::vector<float>().swap(batch.vertices);
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    sivelab::GLSLObject shader;

    shader.addShader( "vertexShader_PrepForPerFragmentBP.glsl", sivelab::GLSLObject::VERTEX_SHADER);

    shader.addShader("fragmentShader_bp.glsl", sivelab::GLSLObject::FRAGMENT_SHADER);

    shader.createProgram();

    const GLint projMatrixID = shader.createUniform("projMatrix");
    const GLint viewMatrixID = shader.createUniform("viewMatrix");
    const GLint modelMatrixID = shader.createUniform("modelMatrix");
    const GLint normalMatrixID = shader.createUniform("normalMatrix");

    const GLint lightPosWorldID = shader.createUniform("lightPosWorld");

    const GLint lightIntensityID = shader.createUniform("lightIntensity");

    const GLint IaID = shader.createUniform("Ia");
    const GLint kaID = shader.createUniform("ka");
    const GLint kdID = shader.createUniform("kd");
    const GLint ksID = shader.createUniform("ks");
    const GLint phongExpID = shader.createUniform("phongExp");

    OpenGLCamera camera(scene.eye, scene.direction, glm::vec3(0,1,0),
                        scene.planeWidth, scene.focalLength, scene.nearPlane, scene.farPlane);

    const glm::vec3 dir = glm::normalize(scene.direction);
    camera.rotateView(glm::degrees(std::atan2(dir.z, dir.x)) + 90.0f,
                      glm::degrees(std::asin(glm::clamp(dir.y, -1.0f, 1.0f))));
    bool rotating = false;
    const glm::vec4 lightPos(scene.lightPosition, 1.0f);

    double previousTime = glfwGetTime();

    //Mouse camera stuff
    double previousMouseX = 0.0;
    double previousMouseY = 0.0;
    bool firstMouse = true;
    bool mouseCaptured = true;

    const float mouseSensitivity = 0.1f;

    // cursor is hid and captured for unlimited movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();


        // Right-click releases the mouse
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && mouseCaptured) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            mouseCaptured = false;
            firstMouse = true;
        }

        // Click inside the window to capture the mouse again
        if (!mouseCaptured && glfwGetWindowAttrib(window, GLFW_HOVERED) && glfwGetWindowAttrib(window, GLFW_FOCUSED) && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            mouseCaptured = true;
            firstMouse = true;
        }

        if (mouseCaptured && glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            if (firstMouse) {
                previousMouseX = mouseX;
                previousMouseY = mouseY;
                firstMouse = false;
            }

            const float mouseDeltaX = static_cast<float>(mouseX - previousMouseX);
            const float mouseDeltaY = static_cast<float>(previousMouseY - mouseY);

            previousMouseX = mouseX;
            previousMouseY = mouseY;

            camera.rotateView(mouseDeltaX * mouseSensitivity, mouseDeltaY * mouseSensitivity);
        } else {
            firstMouse = true;
        }

        const double currentTime = glfwGetTime();
        const float deltaTime = static_cast<float>(currentTime - previousTime);
        previousTime = currentTime;

        const float distance = 16.0f * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.moveForward(distance);
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.moveForward(-distance);
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.moveRight(-distance);
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.moveRight(distance);
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            camera.moveUp(distance);
        }        

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            camera.moveUp(-distance);
        }     

        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && deltaTime > 0.0f) {
            std::cout << "fps: " << 1.0f / deltaTime << '\n';
        }

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            rotating = true;
        }

        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
            rotating = false;
        }
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        if (rotating) {
            for (auto& orbit : scene.orbits) {
                orbit.angle = std::fmod(orbit.angle + orbit.speed * deltaTime, glm::radians(360.0f));
            }
        }

        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

        if (fbWidth == 0 || fbHeight == 0) {
            glfwWaitEvents();
            previousTime = glfwGetTime();
            continue;
        }

        glViewport(0, 0, fbWidth, fbHeight);

        const glm::mat4 projection = camera.getProjectionMatrix(fbWidth, fbHeight);
        const glm::mat4 view = camera.getViewMatrix();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.activate();

        // Shared camera and lighting
        glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(view));
        glUniform4fv(lightPosWorldID, 1, glm::value_ptr(lightPos));
        glUniform3fv(lightIntensityID, 1, glm::value_ptr(scene.lightIntensity));
        glUniform3fv(IaID, 1, glm::value_ptr(scene.ambient));

        glBindVertexArray(sphereVAO);
        for (const Sphere& sphere : scene.spheres) {
            glm::mat4 model(1.0f);
            if (sphere.orbit >= 0) {
                const Orbit& orbit = scene.orbits[sphere.orbit];
                model = glm::translate(model, orbit.pivot);
                model = glm::rotate(model, orbit.angle, orbit.axis);
                model = glm::translate(model, sphere.center - orbit.pivot);
            } else {
                model = glm::translate(model, sphere.center);
            }
            model = glm::scale(model, glm::vec3(sphere.radius));
            const glm::mat4 normalMatrix = glm::transpose(glm::inverse(view * model));
            glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(normalMatrix));
            glUniform3fv(kdID, 1, glm::value_ptr(sphere.material.kd));
            glUniform3fv(kaID, 1, glm::value_ptr(sphere.material.ka));
            glUniform3fv(ksID, 1, glm::value_ptr(sphere.material.ks));
            glUniform1f(phongExpID, sphere.material.exponent);
            glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        }

        for (const auto& batch : scene.triangles) {
            glm::mat4 model(1.0f);
            if (batch.orbit >= 0) {
                const Orbit& orbit = scene.orbits[batch.orbit];
                model = glm::translate(model, orbit.pivot);
                model = glm::rotate(model, orbit.angle, orbit.axis);
                model = glm::translate(model, -orbit.pivot);
            }
            const glm::mat4 normalMatrix = glm::transpose(glm::inverse(view * model));
            glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(normalMatrix));
            glUniform3fv(kdID, 1, glm::value_ptr(batch.material.kd));
            glUniform3fv(kaID, 1, glm::value_ptr(batch.material.ka));
            glUniform3fv(ksID, 1, glm::value_ptr(batch.material.ks));
            glUniform1f(phongExpID, batch.material.exponent);
            glBindVertexArray(batch.vao);
            glDrawArrays(GL_TRIANGLES, 0, batch.vertexCount);
        }

        glBindVertexArray(0);

        shader.deactivate();
        glfwSwapBuffers(window);
    }

    for (const auto& batch : scene.triangles) {
        glDeleteVertexArrays(1, &batch.vao);
        glDeleteBuffers(1, &batch.vbo);
    }
    glDeleteVertexArrays(1, &sphereVAO);
    glDeleteBuffers(1, &sphereVBO);


    

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
