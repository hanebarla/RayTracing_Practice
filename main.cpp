#include <omp.h>

#include <iostream>
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
    auto mat1 = std::make_shared<Diffuse>(Vec3(0.9));

    auto light1 = std::make_shared<Light>(Vec3(0));

    auto ray = Ray(Vec3(0), Vec3(0, 1, 0));
    auto hit = Hit();

    Plane p(Vec3(0, 2, 0), 2.0, Vec3(1,0,0), Vec3(0,0,1), mat1, light1);
    bool a = p.intersect(ray, hit);

    return 0;
}