#ifndef AGGREGATE_H
#define AGGREGATE_H
#include <memory>
#include <vector>
#include "ray.h"
#include "hit.h"
#include "object.h"


class Aggregate {
    public:
        std::vector<std::shared_ptr<Sphere>> spheres;// 球の配列 (平面も追加)

        Aggregate() {};
        Aggregate(const std::vector<std::shared_ptr<Sphere>>& _spheres) :
        spheres(_spheres){};

        // 配列に球を追加
        void add(const std::shared_ptr<Sphere>& s){
            spheres.push_back(s);
        };

        // 最も手前の球との衝突計算
        bool intersect(const Ray& ray, Hit& res) const {
            bool hit = false;
            for(auto s : spheres){
                Hit res_temp;
                if(s->intersect(ray, res_temp)){

                    if(res_temp.t < res.t){
                        hit = true;
                        res = res_temp;
                    }
                }
            }

            return hit;
        };
};

#endif