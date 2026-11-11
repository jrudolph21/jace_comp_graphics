#include <cstdlib>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GLSL.h"
#include "OpenGLCamera.h"

int CheckGLErrors(const char *s)
{
    int errCount = 0;
    return errCount;
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit()) {
        exit (-1);
    }
    // throw std::runtime_error("Error! initialization of glfw failed!");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    int winWidth = 1000;
    float aspectRatio = 1.0; // 16.0 / 9.0; // winWidth / (float)winHeight;
    int winHeight = winWidth / aspectRatio;
    
    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "GLFW Example", NULL, NULL);
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
    glClearColor(0.0, 0.0, 0.0, 1.0);

    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    glViewport(0, 0, fb_width, fb_height);

    //
    // The ortho parameters, in order: left, right, bottom, top, zNear, zFar
    // float halfWidth = 15.0 / 2.0;
    // float halfHeight = halfWidth;
    // float halfHeight_P = halfWidth / aspectRatio;
    // float left = -halfWidth;
    // float right = halfWidth;

    // float bottom = -halfHeight;
    // float top = halfHeight;

    // float near = -10.0f;
    // float far = 10.0f;


    GLint major_version;
    glGetIntegerv(GL_MAJOR_VERSION, &major_version);
    std::cout << "GL_MAJOR_VERSION: " << major_version << std::endl;


    // Creating vertex array buffer
    GLuint m_triangleVBO[1];
    glGenBuffers(1, m_triangleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO[0]);

    std::vector<float> host_VertexBuffer{
        -3.0f, -3.0f, 0.0f,  0.0f, 0.0f, 1.0f,
         3.0f, -3.0f, 0.0f,  0.0f, 0.0f, 1.0f, 
         0.0f,  3.0f, 0.0f,  0.0f, 0.0f, 1.0f 
    };
    
    int numBytes = host_VertexBuffer.size() * sizeof(float);

    // Copy numBytes from buffer to GPU and store in VBO
    glBufferData(GL_ARRAY_BUFFER, numBytes, host_VertexBuffer.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // No longer need CPU memory
    host_VertexBuffer.clear();

    // Create a vertex array objcet (VAO) that will map attributes in our
    // vertex buffer to different location attributes for our shaders
    GLuint m_VAO;
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // VAO details - we only have 1 attribute or location (Postion of the vertex)
    // glEnableVertexAttribArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO[0]);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    // glBindVertexArray(0);

    //More color
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    // glBindVertexArray(0);
    glEnableVertexAttribArray(1);
    //glBindBuffer(GL_ARRAY_BUFFER, m_triangleVBO[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
    glBindVertexArray(0);
    
    
    sivelab::GLSLObject shader;
    shader.addShader( "vertexShader_PrepForPerFragmentBP.glsl", sivelab::GLSLObject::VERTEX_SHADER );
    shader.addShader( "fragmentShader_bp.glsl", sivelab::GLSLObject::FRAGMENT_SHADER );
    shader.createProgram();

 
    GLuint projMatrixID, viewMatrixID, modelMatrixID, normalMatrixID, lightPosWorldID, diffuseComponentID;

    projMatrixID = shader.createUniform( "projMatrix" );
    viewMatrixID = shader.createUniform( "viewMatrix" );
    modelMatrixID = shader.createUniform( "modelMatrix" );
    normalMatrixID = shader.createUniform( "normalMatrix" );
    lightPosWorldID = shader.createUniform ( "lightPosWorld");
    // diffuseComponentID = shader.createUniform("diffuseComponent");

    // // Blinn Phong

    GLuint IaID, kaID, kdID, ksID, phongExpID, lightIntensityID;
    IaID = shader.createUniform("Ia");
    kaID = shader.createUniform("ka");
    kdID = shader.createUniform("kd");
    ksID = shader.createUniform("ks");
    phongExpID = shader.createUniform("phongExp");
    lightIntensityID = shader.createUniform("lightIntensity");


    OpenGLCamera camera(
        glm::vec3(0.0f, 0.0f, 18.0f),  // Eye position
        glm::vec3(0.0f, 0.0f, -1.0f),  // View direction
        glm::vec3(0.0f, 1.0f, 0.0f),   // World up
        1.0f,                          // Image-plane width in world units
        1.0f,                          // Focal length in world units
        0.1f,                          // Near clipping distance
        100.0f                         // Far clipping distance
    );

    double previousTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        const double currentTime = glfwGetTime();
        const float deltaTime =
            static_cast<float>(currentTime - previousTime);
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

        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS &&
            deltaTime > 0.0f) {
            std::cout << "fps: " << 1.0f / deltaTime << '\n';
        }

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        glfwGetFramebufferSize(window, &fb_width, &fb_height);

        if (fb_width == 0 || fb_height == 0) {
            glfwWaitEvents();
            previousTime = glfwGetTime();
            continue;
        }

        glViewport(0, 0, fb_width, fb_height);

        const glm::mat4 projection = camera.getProjectionMatrix(fb_width, fb_height);
        const glm::mat4 view = camera.getViewMatrix();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.activate();

        // Shared camera matrices
        glUniformMatrix4fv(projMatrixID, 1, GL_FALSE, glm::value_ptr(projection));

        glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, glm::value_ptr(view));

        const glm::vec4 lightPos(0.0f, 0.0f, 8.0f, 1.0f);

        glUniform4fv(lightPosWorldID, 1, glm::value_ptr(lightPos));

        // Shared lighting and specular
        glUniform1f(phongExpID, 200.0f);
        glUniform3f(ksID, 1.0f, 1.0f, 1.0f);
        glUniform3f(lightIntensityID, 1.0f, 1.0f, 1.0f);
        glUniform3f(IaID, 1.0f, 1.0f, 1.0f);


        glBindVertexArray(m_VAO);

        // Triangle 1
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 1.0f, 4.0f));

        const float angle1 = glm::radians(80.0f) * static_cast<float>(currentTime);
        model = glm::rotate(model, angle1, glm::vec3(1.0f, 1.0f, 0.0f));

        glm::mat4 normal = glm::transpose(glm::inverse(view * model));

        glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(model));

        glUniformMatrix4fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(normal));

        glUniform3f(kdID, 0.0f, 0.0f, 1.0f);
        glUniform3f(kaID, 0.0f, 0.0f, 0.15f);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Triangle 2
        model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -1.0f, -3.0f));

        const float angle = glm::radians(40.0f) * static_cast<float>(currentTime);
        model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

        normal = glm::transpose(glm::inverse(view * model));

        glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, glm::value_ptr(model));

        glUniformMatrix4fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(normal));

        glUniform3f(kdID, 1.0f, 0.0f, 0.0f);
        glUniform3f(kaID, 0.15f, 0.0f, 0.0f);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);
        shader.deactivate();

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, m_triangleVBO);
    glfwTerminate();
    return 0;
}


