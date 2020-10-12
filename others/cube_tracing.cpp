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
    const int N = 1000;

    Vec3 cubecenter = Vec3(2, 2, -3);
    double cleng = 1.0;
    auto CubePose = CreateCubePos(cubecenter, cleng);

    Image img(512, 512);
    PinholeCamera cam(Vec3(0, 0, 2), Vec3(0, 0, -1), 1);

    auto mat1 = std::make_shared<Diffuse>(Vec3(0.9));
    auto mat2 = std::make_shared<Diffuse>(Vec3(0.1, 0.5, 0.1));
    auto mat3 = std::make_shared<Diffuse>(Vec3(0.8, 0.2, 0.2));
    auto mirror = std::make_shared<Mirror>();

    auto light1 = std::make_shared<Light>(Vec3(0));
    auto light2 = std::make_shared<Light>(Vec3(0.0, 0.0, 0.0));
    auto light3 = std::make_shared<Light>(Vec3(0.0, 0.0, 0.0));

    Aggregate aggregate;

    auto p = std::make_shared<Plane>(Vec3(0, 0, -10), 10, Vec3(1, 0, 0), Vec3(0, 1, 0), mirror, light3);

    auto cube1 = std::make_shared<Plane>(CubePose[0], cleng, Vec3(1, 0, 0), Vec3(0, 0, 1), mat3, light3);
    auto cube2 = std::make_shared<Plane>(CubePose[1], cleng, Vec3(1, 0, 0), Vec3(0, 0, 1), mat3, light3);
    auto cube3 = std::make_shared<Plane>(CubePose[2], cleng, Vec3(0, 1, 0), Vec3(0, 0, 1), mat3, light3);
    auto cube4 = std::make_shared<Plane>(CubePose[3], cleng, Vec3(0, 1, 0), Vec3(0, 0, 1), mat3, light3);
    auto cube5 = std::make_shared<Plane>(CubePose[4], cleng, Vec3(0, 1, 0), Vec3(1, 0, 0), mat3, light3);
    auto cube6 = std::make_shared<Plane>(CubePose[5], cleng, Vec3(0, 1, 0), Vec3(1, 0, 0), mat3, light3);

    aggregate.s_add(
        std::make_shared<Sphere>(Vec3(0, -10001, 0), 10000, mat1, light1));
    aggregate.s_add(std::make_shared<Sphere>(Vec3(-2, 0, -3), 1, mat2, light2));
    aggregate.p_add(p);
    aggregate.p_add(cube1);
    aggregate.p_add(cube2);
    aggregate.p_add(cube3);
    aggregate.p_add(cube4);
    aggregate.p_add(cube5);
    aggregate.p_add(cube6);

#pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int k = 0; k < N; k++) {
                double u = (2.0 * (i + rnd()) - img.width) / img.width;
                double v = (2.0 * (j + rnd()) - img.height) / img.height;

                Ray ray = cam.getRay(-u, -v);

                Vec3 col = radiance(ray, aggregate, Vec3(1));

                img.addPixel(i, j, col);
            }

            if (omp_get_thread_num() == 0) {
                std::cout << double(j + i * img.height) /
                                 (img.width * img.height) * 100
                          << "\r" << std::flush;
            }
        }
    }

    img.divide(N);
    img.gamma_correction();

    img.ppm_output("images\\cube_tracing.ppm");

    return 0;
}