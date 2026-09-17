#include "frameBuffer.h"
#include "camera.h"

int main() {

    frameBuffer fb(500,500);

    PerspectiveCamera p(fb.getWidth(), fb.getHeight(), 1.0, 10);


    for (int x=0; x<fb.getWidth(); ++x) {
        for (int y=0; y<fb.getHeight(); ++y) {
            ray r = p.generateRay( x, y );
            
            // convert ray direction to a color here

            // Normalize the direction vector to get values between -1 and 1
            vec3 dir = r.getDirection();
            dir = dir / dir.length();

            // Map the normalized values to the range [0, 1]
            vec3 ray_dir_color = vec3((dir.x() + 1) / 2, (dir.y() + 1) / 2, (dir.z() + 1) / 2);

            fb.set(x, y, ray_dir_color);
        }

    }

    fb.exportPNG( "defaultCamRayColors.png" );

}
