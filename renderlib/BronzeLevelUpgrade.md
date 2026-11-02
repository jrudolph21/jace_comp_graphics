## [Bronze Level] 

To run a scene:
    - ./renderlib/Debug/json_loader.exe --input ../sceneData/scenes_A/100_spheres.json --width 500 --height 500 --depth 3 --no-bvh --output noBVH.png
    - ./renderlib/Debug/json_loader.exe --input ../sceneData/scenes_A/100_spheres.json --width 500 --height 500 --depth 3 --bvh --output BVH.png

 My results: 
    - Mode: BVH, shapes: 100, Render time: 196009 ms
    - Mode: Linear, shapes: 100 Render, time: 1.03834e+06 ms