#ifndef AGGREGATE_H
#define AGGREGATE_H
#include <memory>
#include <vector>

#include "hit.h"
#include "object.h"
#include "ray.h"

template <size_t N = 0, typename T>
void for_each_intersect(const T& t, const Ray& ray, Hit& res, bool& hit) {
    Hit res_temp;

    if constexpr (N < std::tuple_size<T>::value) {
        auto obs = std::get<N>(t);
        size_t size = obs.size();
        for (auto o : obs) {
            if (o->intersect(ray, res_temp)) {
                if (res_temp.t < res.t) {
                    hit = true;
                    res = res_temp;
                }
            }
        }
        for_each_intersect<N + 1>(t, ray, res, hit);
    }
}

class Aggregate {
   public:
    std::tuple<std::vector<std::shared_ptr<Sphere>>,
               std::vector<std::shared_ptr<Plane>>>
        objects;
    std::vector<std::shared_ptr<Sphere>> spheres;  // 球の配列 (平面も追加)
    std::vector<std::shared_ptr<Plane>> planes;  // 平面の配列

    Aggregate(){
        objects = {spheres, planes};
    };
    Aggregate(const std::tuple<std::vector<std::shared_ptr<Sphere>>,
                               std::vector<std::shared_ptr<Plane>>>& _objects)
        : objects(_objects) {
        spheres = std::get<std::vector<std::shared_ptr<Sphere>>>(objects);
        planes = std::get<std::vector<std::shared_ptr<Plane>>>(objects);
    };

    // 配列に球を追加
    void s_add(const std::shared_ptr<Sphere>& s) {
        spheres.push_back(s);
        objects = {spheres, planes};
    };
    // 配列に球を追加
    void p_add(const std::shared_ptr<Plane>& p) {
        planes.push_back(p);
        objects = {spheres, planes};
    }

    // 最も手前のオブジェクトとの衝突計算
    bool intersect(const Ray& ray, Hit& res) const {
        bool hit = false;
        auto a = std::get<0>(objects);

        for_each_intersect(objects, ray, res, hit);

        return hit;
    };
};

#endif
