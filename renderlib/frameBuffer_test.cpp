#include "frameBuffer.h"
// #include "handleGraphicsArgs.h"

int main() {

    frameBuffer fb(1000, 1000);

    vec3 purple(1, 0.78, 0.18);
    vec3 gold(0.31, 0.15, 0.51);

    fb.clear();

    fb.clear(purple, gold);

    fb.exportPNG("skol.png");
}