#ifndef RADIANCE_H
#define RADIANCE_H
#include "aggregate.h"
#include "hit.h"
#include "ray.h"
#include "vec3.h"

const int MAX_DEPTH = 500;
const double ROULETTE = 0.9;

Vec3 radiance(const Ray &init_ray, const Aggregate &aggregate, const Vec3& sky = Vec3(1)) {
    Vec3 col;
    Vec3 throughput(1);
    Ray ray = init_ray;

    for (int depth = 0; depth < MAX_DEPTH; depth++) {
        Hit res;

        if (aggregate.intersect(ray, res)) {
            Vec3 n = res.hitNormal;

            Vec3 s, t;
            orthonormalBasis(n, s, t);
            Vec3 wo_local = worldToLocal(-ray.direction, s, n, t);

            auto hitMaterial = res.hitSphere->material;
            auto hitLight = res.hitSphere->light;

            // 発光
            col += throughput * hitLight->Le();

            // 方向のサンプリング
            Vec3 brdf;
            Vec3 wi_local;
            double pdf;
            brdf = hitMaterial->sample(wo_local, wi_local, pdf);

            double cos = cosTheta(wi_local);
            Vec3 wi = localToWorld(wi_local, s, n, t);

            throughput *= brdf * cos / pdf;

            ray = Ray(res.hitpos + 0.001 * res.hitNormal, wi);
        } else {
            col += throughput * sky;
            break;
        }

        if (rnd() >= ROULETTE)
            break;
        else
            throughput /= ROULETTE;
    }

    return col;
}

#endif