# Silver Level — OpenGL Scene Renderer

This program loads a JSON scene and rasterizes **spheres and triangles** using OpenGL. It supports Lambertian and Blinn–Phong materials, a point light, a movable camera, and optional orbit animations.

## Requirements

- The shader files `vertexShader_PrepForPerFragmentBP.glsl` and `fragmentShader_bp.glsl` inside `gitHub_shaderFiles` folder
    - These must be place in C:\starterCode\buildVCPkg\fragmentShader_lamb.glsl

## Run the Program

Open a terminal in the directory containing the compiled executable, then specify the JSON scene:


```bash
./silverLevel.exe --input ../sceneData/silverLevelScene.json
```

## Controls

| Input | Action |
|---|---|
| **W** | Move camera forward |
| **S** | Move camera backward |
| **A** | Move camera left |
| **D** | Move camera right |
| **Move mouse** | Look around (when the cursor is captured) |
| **Right mouse button** | Release the mouse cursor |
| **Left mouse button** | Capture the mouse cursor again (click in the focused window) |
| **R** | Start/resume orbit animation |
| **T** | Pause orbit animation |
| **F** | Print the current frame-rate estimate in the terminal |
| **Esc** | Close the application |


## "Side-by-Side" Comparison

The OpenGL image is does not have any shadows as we would assume. It seems like the lighting is much brighter. The postions of the small spheres are much different compared to the ray tracing image. I suspect this because I translate the small sphere to a position in order to make the correct orbiting pattern.

The ray tracing image is much more sharp, with shadows and sharper edges.
