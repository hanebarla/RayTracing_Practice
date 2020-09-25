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
        int obj_mode = 0;
        const Sphere* hitSphere = nullptr;
        const Plane* hitPlane = nullptr;

        Hit(){
            t = 10000;
        };

        int object_mode(){
            if(!hitSphere){
                obj_mode = 1;
            }

            return obj_mode;
        }
};
#endif
