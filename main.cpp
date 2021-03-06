#include <omp.h>

#include <iostream>
#include <memory>

#include "libs\aggregate.h"
#include "libs\camera.h"
#include "libs\hit.h"
#include "libs\image.h"
#include "libs\light.h"
#include "libs\material.h"
#include "libs\object.h"
#include "libs\radiance.h"
#include "libs\random.h"
#include "libs\ray.h"
#include "libs\vec3.h"

int main() {
    const int N = 100;

    Image img(512, 512);
    PinholeCamera cam(Vec3(0, 0, 2), Vec3(0, 0, -1), 1);

    auto mat1 = std::make_shared<Diffuse>(Vec3(0.9));
    auto mat2 = std::make_shared<Diffuse>(Vec3(0.1, 0.5, 0.1));

    auto light1 = std::make_shared<Light>(Vec3(0));
    auto light2 = std::make_shared<Light>(Vec3(0));

    SimpleSky Sky;

    Aggregate aggregate;

    aggregate.s_add(
        std::make_shared<Sphere>(Vec3(0, -10001, 0), 10000, mat1, light1));
    aggregate.s_add(std::make_shared<Sphere>(Vec3(-2, 0, -3), 1, mat2, light2));

#pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            for (int k = 0; k < N; k++) {
                double u = (2.0 * (i + rnd()) - img.width) / img.width;
                double v = (2.0 * (j + rnd()) - img.height) / img.height;

                Ray ray = cam.getRay(-u, -v);

                Vec3 col = radiance(ray, aggregate, Sky);

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

    img.ppm_output("images\\simplesky.ppm");

    return 0;
}