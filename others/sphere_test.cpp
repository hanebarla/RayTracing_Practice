#include "libs\vec3.h"
#include "libs\ray.h"
#include "libs\hit.h"
#include "libs\sphere.h"


int main(){
    Sphere s(Vec3(0, 0, 0), 1);
    Ray ray(Vec3(0, 0, -3), Vec3(0, 0, 1));
    Hit res;

    if(s.intersect(ray, res)){
        std::cout << res.hitpos << std::endl;
    }

    return 0;
}
