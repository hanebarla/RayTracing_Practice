#ifndef HIT_H
#define HIT_H
#include "vec3.h"


class Sphere;
class Plane;


class Hit {
    public:
        double t;
        Vec3 hitpos;
        Vec3 hitNormal;
        const Sphere* hitSphere = nullptr;
        const Plane* hitPlane = nullptr;

        Hit(){
            t = 10000;
        };
};
#endif
