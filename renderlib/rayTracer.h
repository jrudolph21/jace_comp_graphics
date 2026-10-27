#pragma once

#include "scene.h"
#include "frameBuffer.h"

class rayTracer {

    public:

        void render(const Scene& scene, frameBuffer& fb, int cameraIndex = 0, int depth = 3, bvhNode* root = nullptr);

};

