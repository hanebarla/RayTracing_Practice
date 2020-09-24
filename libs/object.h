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

class Plane : public Object {
   public:
    Vec3 center;
    Vec3 edge1;
    Vec3 edge2;
    Vec3 origin;
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
          };

    bool intersect(const Ray& ray, Hit& res) const {
        std::vector<double>answer = Cramer(origin, edge1, edge2, ray);
        double alpha = answer[0];
        double beta = answer[1];
        double t = answer[2];
        std::cout << "alpha: " << alpha << ", beta: " << beta << ", t: " << t << std::endl;
        return true;
    }
};

#endif
