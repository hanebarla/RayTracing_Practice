#ifndef OBJECT_H
#define OBJECT_H
#include <cmath>

#include "hit.h"
#include "light.h"
#include "material.h"
#include "ray.h"
#include "util.h"
#include "vec3.h"

class Object {
   public:
    virtual bool intersect(const Ray& ray, Hit& hit) {return false;};
};


/*
球オブジェクト
-----------
メンバ変数
    Vec3 center: 中心座標
    double radius: 半径
    Material _material: マテリアル
    Light _light: 放射
*/
class Sphere : public Object {
   public:
    Vec3 center;
    double radius;
    std::shared_ptr<Material> material;
    std::shared_ptr<Light> light;

    Sphere(const Vec3& _center, double _radius,
           const std::shared_ptr<Material>& _material)
        : center(_center), radius(_radius), material(_material){};

    Sphere(const Vec3& _center, double _radius,
           const std::shared_ptr<Material>& _material,
           const std::shared_ptr<Light>& _light)
        : center(_center),
          radius(_radius),
          material(_material),
          light(_light){};

    bool intersect(const Ray& ray, Hit& res) const {
        double b = dot(ray.direction, ray.origin - center);
        double c = (ray.origin - center).length2() - radius * radius;

        double D = b * b - c;

        double t1 = -b - std::sqrt(D);
        double t2 = -b + std::sqrt(D);

        if ((t1 > 10000) | (t2 < 0)) {
            return false;
        }

        double t = t1;
        if (t < 0) {
            t = t2;
            if (t2 > 10000) {
                return false;
            }
        }

        res.t = t;
        res.hitpos = ray(t);
        res.hitNormal = normalize(res.hitpos - center);
        res.hitSphere = this;

        return true;
    };
};

// 平面オブジェクト
class Plane : public Object {
   public:
    Vec3 center;
    Vec3 edge1;
    Vec3 edge2;
    Vec3 origin;
    Vec3 normal1;
    Vec3 normal2;
    double length;
    std::shared_ptr<Material> material;
    std::shared_ptr<Light> light;

    Plane(const Vec3& _center, double _length, const Vec3& _edge1, const Vec3& _edge2,
          const std::shared_ptr<Material>& _material,
          const std::shared_ptr<Light>& _light)
        : center(_center),
          length(_length),
          edge1(_edge1),
          edge2(_edge2),
          material(_material),
          light(_light){
              edge1 = normalize(edge1);
              edge2 = normalize(edge2);
              origin = center - edge1*(length/2) - edge2*(length/2);
              normal1 = cross(edge1, edge2);
              normal2 = cross(edge2, edge1);
          };

    bool intersect(const Ray& ray, Hit& res) const {
        std::vector<double>answer = Cramer(origin, edge1, edge2, ray);
        double alpha = answer[0];
        double beta = answer[1];
        double t = answer[2];

        if((alpha > length) || (beta > length)){
            return false;
        }

        if((alpha < 0) || (beta < 0)){
            return false;
        }

        if(t < 0){
            return false;
        }

        Vec3 normal = normal1;
        double theta = dot(normal, ray.direction);
        if(theta <= 0.0){
            normal = normal2;
        }

        res.t = t;
        res.hitpos = ray(t);
        res.hitNormal = normalize(normal);
        res.hitPlane = this;
        return true;
    }
};

#endif
