#include <cstdlib>
#include <iostream>
#include <vector>
#include <png++/png.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLSL.h"
#include "OpenGLCamera.h"
#include "OpenGLTriangle.h"

int main()
{
    if (!glfwInit()) {
        exit (-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(1000, 1000, "Silver Level", NULL, NULL);

    if (!window) {
        std::cerr << "GLFW did not create a window!" << std::endl;
        
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err=glewInit();
    if(err != GLEW_OK) {
        std::cerr <<"GLEW Error! glewInit failed, exiting."<< std::endl;
        exit(EXIT_FAILURE);
    }


    const GLubyte* renderer = glGetString (GL_RENDERER);
    const GLubyte* version = glGetString (GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.02f, 0.02f, 0.025f, 1.0f);

    std::string texFilename = "C:/Users/racer/comp_graphics/starterCode/sceneData/scenes_B/images/textureCalibrationNumberGrid.png";
    std::cout << "Reading texture map data from file: " << texFilename << std::endl;
    png::image<png::rgb_pixel> texPNGImage;
    texPNGImage.read(texFilename);

    int pngWidth = texPNGImage.get_width();
    int pngHeight = texPNGImage.get_height();

    std::vector<float> texData(pngHeight * pngWidth * 3);

    size_t idx = 0;
    for (size_t row = 0; row < pngHeight; ++row) {
        for (size_t col = 0; col < pngWidth; ++col) {
            png::rgb_pixel pixel = texPNGImage[pngHeight - row - 1][col]; // <-- notice the flip of height!!!
            texData[idx++] = pixel.red / 255.0f;
            texData[idx++] = pixel.green / 255.0f;
            texData[idx++] = pixel.blue / 255.0f;
        }
    }

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pngWidth, pngHeight, 0, GL_RGB, GL_FLOAT, texData.data());
    glBindTexture(GL_TEXTURE_2D, 0);

    struct vertexData
    {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 texture;
    };

    const vertexData triangleVertices[] = {

        // Triangle 1
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.5f}},
        {{ 0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.5f, 0.5f}},
        {{ 0.5f,  0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}},

        // Triangle 2
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.5f}},
        {{ 0.5f,  0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}},
        {{-0.5f,  0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},

        // Triangle 3
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.5f, 0.5f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.5f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},

        // Triangle 4
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.5f, 0.5f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.5f, 1.0f}},

        // Triangle 5
        {{0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.5f, 0.0f}},
        {{0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.5f, 0.5f}},

        // Triangle 6
        {{0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.5f, 0.5f}},
        {{0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.5f}},

        // Triangle 7
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.5f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.5f}},

        // Triangle 8
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.5f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.5f}},
        {{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.5f, 0.5f}},

        // Triangle 9
        {{-0.5f, 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.5f}},
        {{ 0.5f, 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.5f, 0.5f}},
        {{ 0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.5f, 1.0f}},

        // Triangle 10
        {{-0.5f, 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.5f}},
        {{ 0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.5f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},

        // Triangle 11
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.5f, 0.5f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.5f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},

        // Triangle 12
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.5f, 0.5f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.5f, 1.0f}}
    };

    // std::vector< vertexData > modelData;
    // int numBytes = modelData.size() * sizeof(vertexData);
    // glBufferData(GL_ARRAY_BUFFER, numBytes, modelData.data(), GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint triangleVBO;
    GLuint triangleVAO;

    glGenVertexArrays(1, &triangleVAO);
    glGenBuffers(1, &triangleVBO);

    glBindVertexArray(triangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));

    // Texture
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // std::vector<float> sphereOfTriangles = createSphere(4, .25f);

    // // How many vertices are stored in the VBO
    // const GLsizei vertexCount = static_cast<GLsizei>(sphereOfTriangles.size() / 8);

    // GLuint sphereVBO;
    // GLuint sphereVAO;

    // glGenBuffers(1, &sphereVBO);
    // glGenVertexArrays(1, &sphereVAO);

    // glBindVertexArray(sphereVAO);
    // glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);

    // glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sphereOfTriangles.size() * sizeof(float)), sphereOfTriangles.data(), GL_STATIC_DRAW);

    // // Attribute 0: vertex position
    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

    // // Attribute 1: vertex normal
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));

    // // Attribute 2: texture coordinates
    // glEnableVertexAttribArray(2);
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));

    // glBindVertexArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    // // No longer need CPU vertex data
    // sphereOfTriangles.clear();

    // // Attribute 0: vertex position.
    // glEnableVertexAttribArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

    // // Attribute 1: vertex normal.
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    // glBindVertexArray(0);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    sivelab::GLSLObject shader;

    shader.addShader( "vertexShader_PrepForPerFragmentTexture.glsl", sivelab::GLSLObject::VERTEX_SHADER);

    shader.addShader("fragmentShader_bp_texture.glsl", sivelab::GLSLObject::FRAGMENT_SHADER);

    shader.createProgram();

    const GLint projMatrixID = shader.createUniform("projMatrix");
    const GLint viewMatrixID = shader.createUniform("viewMatrix");
    const GLint modelMatrixID = shader.createUniform("modelMatrix");
    const GLint normalMatrixID = shader.createUniform("normalMatrix");
    const GLint textUnitID = shader.createUniform("textureUnit"); 

    const GLint lightPosWorldID = shader.createUniform("lightPosWorld");

    const GLint lightIntensityID = shader.createUniform("lightIntensity");

    const GLint IaID = shader.createUniform("Ia");
    const GLint kaID = shader.createUniform("ka");
    const GLint kdID = shader.createUniform("kd");
    const GLint ksID = shader.createUniform("ks");
    const GLint phongExpID = shader.createUniform("phongExp");

    OpenGLCamera camera(
        glm::vec3(0.0f, 0.0f, 12.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        1.0f,
        1.0f,
        0.1f,
        100.0f
    );

    bool rotating = false;
    // Different starting points along each orbit.
    float angleA = glm::radians(-35.0f);
    const float speedA = glm::radians(20.0f);

    const glm::vec3 pivot(0.0f, 0.0f, 0.0f);

    // Central sphere black
    const glm::vec3 sphere0Position(1.0f, 1.0f, 1.0f);

    // // Orbit A: red and green
    // const glm::vec3 sphere1Position(3.5f, 0.0f, 0.0f);
    // const glm::vec3 sphere2Position(-3.5f, 0.0f, 0.0f);
    // const glm::vec3 axisA = glm::normalize(glm::vec3(0.0f, 1.0f, 0.35f));

    // // Orbit B: blue and yellow
    // const glm::vec3 sphere3Position(2.1f, 2.8f, 0.0f);
    // const glm::vec3 sphere4Position(-2.1f, -2.8f, 0.0f);
    // const glm::vec3 axisB = glm::normalize(glm::vec3(-0.8f, 0.6f, 0.35f));

    // // Orbit C: orange and purple
    // const glm::vec3 sphere5Position(-2.1f, 2.8f, 0.0f);
    // const glm::vec3 sphere6Position(2.1f, -2.8f, 0.0f);
    // const glm::vec3 axisC = glm::normalize(glm::vec3(0.8f, 0.6f, 0.35f));

    // light position
    const glm::vec4 lightPos(0.0f, 0.0f, 8.0f, 1.0f);

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


        // Escape releases the mouse.
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && mouseCaptured) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            mouseCaptured = false;
            firstMouse = true;
        }

        // Click inside the window to capture the mouse again.
        if (!mouseCaptured && glfwGetWindowAttrib(window, GLFW_HOVERED) && glfwGetWindowAttrib(window, GLFW_FOCUSED) && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            mouseCaptured = true;
            firstMouse = true;
        }

        // Update the camera while the mouse is captured.
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

        const float distance = 4.0f * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.moveForward(distance);

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.moveForward(-distance);

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.moveRight(-distance);

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.moveRight(distance);

        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && deltaTime > 0.0f)
            std::cout << "fps: " << 1.0f / deltaTime << '\n';

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            rotating = true;

        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
            rotating = false;
        
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        if (rotating) {
            angleA += speedA * deltaTime;
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

        // Shared camera and lighting.
        glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(view));
        glUniform4fv(lightPosWorldID, 1, glm::value_ptr(lightPos));
        glUniform3f(lightIntensityID, 1.0f, 1.0f, 1.0f);
        glUniform3f(IaID, 1.0f, 1.0f, 1.0f);

        // Cube
        glm::mat4 model(1.0f);

        model = glm::scale(model, glm::vec3(4.0f));

        model = glm::rotate(model, angleA, glm::vec3(0.0f, 1.0f, 0.0f));

        const glm::mat4 normalMatrix = glm::transpose(glm::inverse(view * model));

        glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(model));

        glUniformMatrix4fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(normalMatrix));

        glUniform3f(kdID, 1.0f, 1.0f, 1.0f);
        glUniform3f(kaID, 0.1f, 0.1f, 0.1f);
        glUniform3f(ksID, 0.2f, 0.2f, 0.2f);
        glUniform1f(phongExpID, 512.0f);

        // Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texID);
        glUniform1i(textUnitID, 0);

        // Draw cube
        glBindVertexArray(triangleVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        shader.deactivate();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &triangleVAO);
    glDeleteBuffers(1, &triangleVBO);
    

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}