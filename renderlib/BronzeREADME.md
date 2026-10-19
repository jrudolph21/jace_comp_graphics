## [Bronze Level] 

To run my scene:
 - uncomment "bronze_level.cpp" in the CMakeLists.txt under the renderlib folder
 -  ./renderlib/Debug/frameBuffer.exe --width 500 --height 500 --position 0 3 3 --direction 0 -0.4 -1 --focal-length 0.4 --cam-width 0.5    --depth 5 --output render.png


To test and compare the ray-triangle intersection:
 - uncomment "triangle_benchmark.cpp" in the CMakeLists.txt under the renderlib folder
 - run (cmake --build .)
 - run ./renderlib/Debug/framebuffer.exe

 My results, not sure why the new method is slower. 

    Run 1: old = 30599.3 ms, Moller = 41594.3 ms

    Old average render time: 30599.3 ms
    Moller average render time: 41594.3 ms
    Render speed ratio (old / Moller): 0.735661x