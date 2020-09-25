#include <omp.h>

#include <iostream>
#include <tuple>
#include <memory>

#include "libs\aggregate.h"
#include "libs\camera.h"
#include "libs\hit.h"
#include "libs\image.h"
#include "libs\light.h"
#include "libs\material.h"
#include "libs\radiance.h"
#include "libs\random.h"
#include "libs\ray.h"
#include "libs\object.h"
#include "libs\vec3.h"

int main() {
    const int N = 200;
    Image img(512, 512);
    PinholeCamera cam(Vec3(0, 0, 1), Vec3(0, 0, -1), 1);

    Aggregate aggregate;

    auto mat1 = std::make_shared<Diffuse>(Vec3(0.9));

    auto light1 = std::make_shared<Light>(Vec3(0));

    auto ray = Ray(Vec3(0), Vec3(0, 1, 0));
    auto hit = Hit();

    auto p = std::make_shared<Plane>(Vec3(0, 9, 0), 2.0, Vec3(1,0,0), Vec3(0,0,1), mat1, light1);
    auto s = std::make_shared<Sphere>(Vec3(0, 5, 0), 3, mat1, light1);

    aggregate.p_add(p);
    aggregate.s_add(s);

    bool a = aggregate.intersect(ray, hit);
    std::cout << a << std::endl;

    return 0;
}