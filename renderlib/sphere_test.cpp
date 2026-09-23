#include "frameBuffer.h"
#include "camera.h"
#include "shape.h"

int main() {

    frameBuffer fb(2000,1000);

    vec3 position(0, 0, 0);
    vec3 viewDirection(0, 0, -1);

    vec3 white(1,1,1);
    vec3 red(1,0,0);
    vec3 black(0,0,0);

    PerspectiveCamera p(viewDirection, position, fb.getWidth(), fb.getHeight(), 1.0, 0.5);

    Sphere s0(vec3(0,0,-100), 15.00);


    for (int x=0; x<fb.getWidth(); ++x) {
        for (int y=0; y<fb.getHeight(); ++y) {
            ray r = p.generateRay( x, y );
            
            // // convert ray direction to a color here

            // // Normalize the direction vector to get values between -1 and 1
            // vec3 dir = r.getDirection();
            // dir = dir / dir.length();

            // // Map the normalized values to the range [0, 1]
            // vec3 ray_dir_color = vec3((dir.x() + 1) / 2, (dir.y() + 1) / 2, (dir.z() + 1) / 2);

            // fb.set(x, y, ray_dir_color);

            if(s0.intersect(r)) {
                fb.set(x,y,red);
            }
            else {
                fb.set(x,y,white);
            }
        }

    }

    fb.exportPNG( "SphereCamRayColorsTEST_rectangle.png" );

}