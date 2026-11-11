include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable (glfwExample
  glfwExample.cpp 
  GLSL.cpp GLSL.h
)

add_executable(matrix_lab
    matrices_lab.cpp
    GLSL.cpp GLSL.h
    OpenGLCamera.h
)

add_executable(lamb_shader
    lamb_shader.cpp
    GLSL.cpp GLSL.h
    OpenGLCamera.h
)

add_executable(bp_shader
    bp_shader.cpp
    GLSL.cpp GLSL.h
    OpenGLCamera.h
)

add_executable(norm_shader
    normal_shader.cpp
    GLSL.cpp GLSL.h
    OpenGLCamera.h
)

target_link_libraries (glfwExample PRIVATE GLEW::GLEW)
target_link_libraries (glfwExample PRIVATE glfw)

if(APPLE)
    target_link_libraries(glfwExample PRIVATE
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
else()
    target_link_libraries(glfwExample PRIVATE OpenGL::GL)
endif()

target_link_libraries (glfwExample PRIVATE PNG::PNG)
target_link_libraries (glfwExample PRIVATE ZLIB::ZLIB)



target_link_libraries (matrix_lab PRIVATE GLEW::GLEW)
target_link_libraries (matrix_lab PRIVATE glfw)

if(APPLE)
    target_link_libraries(matrix_lab PRIVATE
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
else()
    target_link_libraries(matrix_lab PRIVATE OpenGL::GL)
endif()

target_link_libraries (matrix_lab PRIVATE PNG::PNG)
target_link_libraries (matrix_lab PRIVATE ZLIB::ZLIB)



target_link_libraries (lamb_shader PRIVATE GLEW::GLEW)
target_link_libraries (lamb_shader PRIVATE glfw)

if(APPLE)
    target_link_libraries(lamb_shader PRIVATE
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
else()
    target_link_libraries(lamb_shader PRIVATE OpenGL::GL)
endif()

target_link_libraries (lamb_shader PRIVATE PNG::PNG)
target_link_libraries (lamb_shader PRIVATE ZLIB::ZLIB)



target_link_libraries (bp_shader PRIVATE GLEW::GLEW)
target_link_libraries (bp_shader PRIVATE glfw)

if(APPLE)
    target_link_libraries(bp_shader PRIVATE
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
else()
    target_link_libraries(bp_shader PRIVATE OpenGL::GL)
endif()

target_link_libraries (bp_shader PRIVATE PNG::PNG)
target_link_libraries (bp_shader PRIVATE ZLIB::ZLIB)



target_link_libraries (norm_shader PRIVATE GLEW::GLEW)
target_link_libraries (norm_shader PRIVATE glfw)

if(APPLE)
    target_link_libraries(norm_shader PRIVATE
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
else()
    target_link_libraries(norm_shader PRIVATE OpenGL::GL)
endif()

target_link_libraries (norm_shader PRIVATE PNG::PNG)
target_link_libraries (norm_shader PRIVATE ZLIB::ZLIB)


