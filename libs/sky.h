#ifndef SKY_H
#define SKY_H
#include "ray.h"
#include "vec3.h"

class Sky {
   public:
    // 受け取ったレイの方からくる空の放射輝度の値を返す
    // 戻り値: 空の放射輝度
    // ray: 放射輝度をとる方向のレイ
    virtual Vec3 getRadiance(const Ray& ray) const = 0;
};

class Uniformsky : public Sky {
   public:
    Vec3 color;  // 空の色

    Uniformsky(const Vec3& _color) : color(_color){};

    Vec3 getRadiance(const Ray& ray) const { return color; };
};

class SimpleSky : public Sky {
   public:
    SimpleSky(){};

    Vec3 getRadiance(const Ray& ray) const {
        double t = (ray.direction.y + 1) / 2;
        return (1 - t) * Vec3(1) + t * Vec3(0.7, 0.8, 1);
    };
};
#endif