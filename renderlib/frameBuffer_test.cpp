#include "frameBuffer.h"
// #include "handleGraphicsArgs.h"

int main() {

    frameBuffer fb(1000, 1000);

    vec3 gold(1, 0.78, 0.18);
    vec3 purple(0.31, 0.15, 0.51);
    // vec3 black(0,0,0);

    fb.clear(purple, gold);

    // for (int x = 200; x < 300; x++) {
    //     for (int y = 200; y < 800; y++) {
    //         fb.set(x, y, black);
    //     }
    // }
    
    //   for (int x = 700; x < 800; x++) {
    //     for (int y = 200; y < 800; y++) {
    //         fb.set(x, y, black);
    //     }
    // }

    //    for (int x = 500; x < 600; x++) {
    //     for (int y = 200; y < 800; y++) {
    //         fb.set(x, y, black);
    //     }
    // }

    // for (int x = 500; x < 800; x++) {
    //     for (int y = 700; y < 800; y++) {
    //         fb.set(x, y, black);
    //     }
    // }

    // for (int x = 500; x < 800; x++) {
    //     for (int y = 200; y < 300; y++) {
    //         fb.set(x, y, black);
    //     }
    // }

    // for (int x = 500; x < 800; x++) {
    //     for (int y = 450; y < 550; y++) {
    //         fb.set(x, y, black);
    //     }
    // }

    fb.exportPNG("skol.png");
}