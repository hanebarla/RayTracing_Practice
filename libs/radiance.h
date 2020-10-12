#ifndef RADIANCE_H
#define RADIANCE_H
#include "aggregate.h"
#include "hit.h"
#include "ray.h"
#include "vec3.h"
#include "sky.h"

const int MAX_DEPTH = 500;
const double ROULETTE = 0.9;

Vec3 cala_col(int mode, const Hit res, Vec3 throughput){
    if(mode==0){
        auto hitMaterial = res.hitSphere->material;
        auto hitLight = res.hitSphere->light;

        return throughput * hitLight->Le();
    }
    else{
        auto hitMaterial = res.hitPlane->material;
        auto hitLight = res.hitPlane->light;

        return throughput * hitLight->Le();
    }
}

Vec3 cala_brdf(int mode, const Hit res, Vec3& wo_local, Vec3& wi_local, double& pdf){
    if(mode==0){
        auto hitMaterial = res.hitSphere->material;
        return hitMaterial->sample(wo_local, wi_local, pdf);
    }
    else{
        auto hitMaterial = res.hitPlane->material;
        return hitMaterial->sample(wo_local, wi_local, pdf);
    }
}

template <typename T>
Vec3 radiance(const Ray &init_ray, const Aggregate &aggregate,
              const T &sky ) {
    Vec3 col;
    Vec3 throughput(1);
    Ray ray = init_ray;

    for (int depth = 0; depth < MAX_DEPTH; depth++) {
        Hit res;

        if (aggregate.intersect(ray, res)) {
            int mode = res.object_mode();
            Vec3 n = res.hitNormal;

            Vec3 s, t;
            orthonormalBasis(n, s, t);
            Vec3 wo_local = worldToLocal(-ray.direction, s, n, t);

            // 発光
            col += cala_col(mode, res, throughput);

            // 方向のサンプリング
            Vec3 brdf;
            Vec3 wi_local;
            double pdf;
            brdf = cala_brdf(mode, res, wo_local, wi_local, pdf);

            double cos = cosTheta(wi_local);
            Vec3 wi = localToWorld(wi_local, s, n, t);

            throughput *= brdf * cos / pdf;

            ray = Ray(res.hitpos + 0.001 * res.hitNormal, wi);
        } else {
            col += throughput * sky.getRadiance(ray);
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