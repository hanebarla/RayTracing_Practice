#include <memory>
#include "libs\vec3.h"
#include "libs\ray.h"
#include "libs\hit.h"
#include "libs\sphere.h"
#include "libs\camera.h"
#include "libs\image.h"
#include "libs\aggregate.h"


int main(){
    Image img(512, 512);
    PinholeCamera cam(Vec3(0, 0, 0), Vec3(0, 0, -1), 1);

    Aggregate aggregate;

    aggregate.add(std::make_shared<Sphere>(Vec3(0, 0, -3), 1));
    aggregate.add(std::make_shared<Sphere>(Vec3(0, -10001, 0), 10000));

    for(int i=0; i<img.width; i++){
        for(int j=0; j<img.height; j++){
            double u = (2.0*i - img.width)/img.width;
            double v = (2.0*j - img.height)/img.height;

            Vec3 sunDir = normalize(Vec3(1, 1, 1));

            Ray ray = cam.getRay(-u, -v);

            Vec3 col;
            Hit res;

            if(aggregate.intersect(ray, res)){
                Ray shadowRay(res.hitpos + 0.001*res.hitNormal, sunDir);
                Hit res_temp;

                if(!aggregate.intersect(shadowRay, res_temp)){
                    col = std::max(dot(res.hitNormal, sunDir), 0.0) * Vec3(1);
                }
            }

            img.setPixel(i, j, col);
        }
    }

    img.ppm_output("images\\draw_shadow.ppm");

    return 0;
}
