#include "frameBuffer.h"
#include "camera.h"
#include "shape.h"
#include <limits>

int main() {

    frameBuffer fb(200, 200);

    vec3 position(0, 0, 0);
    vec3 viewDirection(0, 0, -1);

    vec3 white(1, 1, 1);
    vec3 red(1, 0, 0);
    vec3 blue(0, 0, 1);
    vec3 green(0, 0.7, 0);
    vec3 orange(1, 0.5, 0);
    vec3 purple(0.6, 0, 0.8);
    vec3 black(0,0,0);

    PerspectiveCamera p(
        viewDirection, position,
        fb.getWidth(), fb.getHeight(),
        1.0, 0.5
    );

    // Sphere s0(vec3(-11, 9, -100), 7.0);
    Sphere s1(vec3( 0, 0, -6), 0.2);

    Triangle t1(
        vec3(-1.2, -0.2, -7),
        vec3(0.8, -0.5, -5),
        vec3(0.9,  0, -5)
    );

    Triangle t2(
        vec3(0.773205, -0.93923, -7),
        vec3(0.0330127, 0.94282, -5),
        vec3(-0.45, 0.779423, -5)
    );

    Triangle t3(
        vec3(0.426795, 1.13923, -7),
        vec3(-0.833013, -0.44282, -5),
        vec3(-0.45, -0.779423, -5)
    );

    // Each shape is paired with its display color.
    struct SceneObject {
        Shape* shape;
        vec3 color;
    };

    SceneObject objects[] = {
        { &t1, red },
        { &t2, green },
        { &t3, blue },
        { &s1, white}
    };

    for (int x = 0; x < fb.getWidth(); ++x) {
        for (int y = 0; y < fb.getHeight(); ++y) {

            ray r = p.generateRay(x, y);

            const float tmin = 1.0f;
            float tmax = std::numeric_limits<float>::infinity();
            HitStruct hit;

            vec3 pixelColor = white;
            
            // vec3 direction = r.getDirection();
            // direction = direction / direction.length();

            // vec3 pixelColor = 0.5 * (direction + vec3(1, 1, 1));

            for (const SceneObject& object : objects) {
                if (object.shape->intersect(r, tmin, tmax, hit)) {
                    pixelColor = object.color;
                }
            }

            fb.set(x, y, pixelColor);
        }
    }

    fb.exportPNG("YOverlapping_Triangles_Sphere.png");
}