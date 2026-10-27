include_directories(${CMAKE_SOURCE_DIR}/src)

add_library (renderlib
  vec3.h
  frameBuffer.h
  frameBuffer.cpp
  ray.h 
  camera.h
  camera.cpp
  shape.h
  shape.cpp
  HitStruct.h
  PointLight.h
  shader.h
  shader.cpp
  scene.h
  scene.cpp
  rayTracer.h
  rayTracer.cpp
  ISceneLoader.h
  SceneLoader.h 
  SceneParser_JSON.cpp 
  SceneParser_JSON.h
  AABB.h
  )

target_link_libraries(renderlib PRIVATE PNG::PNG)
target_link_libraries(renderlib PRIVATE ZLIB::ZLIB)
target_link_libraries(renderlib PUBLIC nlohmann_json::nlohmann_json glm::glm)

add_executable( frameBuffer
  #frameBuffer_test.cpp
  #camera_test.cpp
  #sphere_test.cpp
  # main.cpp
  # triangle_benchmark.cpp
  # progress_lab.cpp
  # bronze_level.cpp
)

add_executable( progress_lab
  # bronze_level.cpp 
)

add_executable( json_loader
  json_loader.cpp
)

target_link_libraries(frameBuffer PRIVATE renderlib)
target_link_libraries(frameBuffer PRIVATE cs4212-util)
target_link_libraries(frameBuffer PRIVATE ${Boost_PROGRAM_OPTIONS_LIBRARIES})

target_link_libraries(progress_lab PRIVATE renderlib)
target_link_libraries(progress_lab PRIVATE cs4212-util)
target_link_libraries(progress_lab PRIVATE ${Boost_PROGRAM_OPTIONS_LIBRARIES})

target_link_libraries(json_loader PRIVATE renderlib)
target_link_libraries(json_loader PRIVATE cs4212-util)
target_link_libraries(json_loader PRIVATE ${Boost_PROGRAM_OPTIONS_LIBRARIES})
