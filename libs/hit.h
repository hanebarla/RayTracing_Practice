#ifndef HIT_H
#define HIT_H
#include "vec3.h"


class Sphere;


class Hit {
    public:
        double t;
        Vec3 hitpos;
        Vec3 hitNormal;
        const Sphere* hitSphere;

        Hit(){
            t = 10000;
        };
};
#endif
