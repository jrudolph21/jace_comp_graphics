#include "frameBuffer.h"
#include "png++/png.hpp"
#include <algorithm>

frameBuffer::frameBuffer() : width(100), height(100) {
    fb.resize(width * height);
}

frameBuffer::frameBuffer(int w, int h) : width(w), height(h) {
    fb.resize(width * height);
}

void frameBuffer::clear() {
    for (int i = 0; i < width * height; i++) {
        fb[i] = vec3(0.0, 0.0, 0.0);
    }
}

void frameBuffer::clear(vec3 rgb1) {
    for (int i=0; i<width*height; ++i) {
        fb[i] = vec3(rgb1.x(), rgb1.y(), rgb1.z());
    }
}

void frameBuffer::clear(vec3 rgb1, vec3 rgb2) {
    for (int i=0; i<height; ++i) {
        for (int j=0; j<width; ++j) {
            float t = float(i+1) / float(height); 
            fb[i*width +j] = rgb1 * (1.0 - t) + rgb2 * t;
        }
    }
}

void frameBuffer::set(int x, int y, vec3 rgb) {
    if ((x>0 && x<width+1) && (y>0 && y<height+1)) {
        fb[y*width + x] = rgb;
    }
}

void frameBuffer::exportPNG(std::string filename) {

    png::image< png::rgb_pixel > imData( width, height);

    for (size_t y = 0; y < imData.get_height(); ++y) {
        for (size_t x = 0; x < imData.get_width(); ++x) {
            imData[y][x] = png::rgb_pixel( static_cast<png::byte>(std::clamp(fb[y*width + x].x() * 255.0, 0.0, 255.0)),
                                         static_cast<png::byte>(std::clamp(fb[y*width + x].y() * 255.0, 0.0, 255.0)),
                                         static_cast<png::byte>(std::clamp(fb[y*width + x].z() * 255.0, 0.0, 255.0)) );
	    }
    }
    imData.write( filename );
}
