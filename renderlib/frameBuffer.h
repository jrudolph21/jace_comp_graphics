/* Frame Buffer for rendering */

#pragma once

#include <vector>
#include <cstring>
#include "vec3.h"

class frameBuffer {

    public:
        frameBuffer();

        frameBuffer(int w, int h);

        void clear();

        void clear(vec3 rgb1);

        void clear(vec3 rgb1, vec3 rgb2);

        void set(int x, int y, vec3 rgb);

        void exportPNG(std::string filename);

    private:
        std::vector<vec3> fb;
        int width;
        int height;

};
