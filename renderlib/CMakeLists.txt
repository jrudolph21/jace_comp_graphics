include_directories(${CMAKE_SOURCE_DIR}/src)

add_library (renderlib
  vec3.h
  frameBuffer.h
  frameBuffer.cpp
  ray.h 
)
target_link_libraries(renderlib PRIVATE PNG::PNG)
target_link_libraries(renderlib PRIVATE ZLIB::ZLIB)

add_executable( frameBuffer
  frameBuffer_test.cpp
)
target_link_libraries(frameBuffer PRIVATE renderlib)
target_link_libraries(frameBuffer PRIVATE cs4212-util)
target_link_libraries(frameBuffer PRIVATE ${Boost_PROGRAM_OPTIONS_LIBRARIES})

