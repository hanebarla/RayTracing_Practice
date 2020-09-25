#ifndef MATERIAL_H
#define MATERIAL_H
#define _USE_MATH_DEFINES
#include "vec3.h"
#include "random.h"
#include <cmath>
# define M_PI 3.141592653589793238462643383279502884L

class Material {
    public:
        //BRDFから方向をサンプリング
        //戻り値: BRDFの値
        //wi: サンプリングされた方向
        //pdf: サンプリングされた方向の確率密度関数の値
        virtual Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const = 0;
};

class Diffuse : public Material {
    public:
        Vec3 rho;

        Diffuse(const Vec3& _rho) : rho(_rho) {};

        Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const {
            double u = rnd();
            double v = rnd();
            double theta = 0.5 * std::acos(1 - 2*u);
            double phi = 2*M_PI*v;

            double x = std::cos(phi)*std::sin(theta);
            double y = std::cos(theta);
            double z = std::sin(phi)*std::sin(theta);

            wi = Vec3(x, y, z);

            pdf = std::cos(theta)/M_PI;

            return rho/M_PI;
        };
};

// 反射ベクトル: v 反射させるベクトル, n 法線ベクトル
Vec3 reflect(const Vec3& v, const Vec3& n){
    return -v + 2*dot(v, n)*n;
}

// ローカル座標系のベクトルからcosTheta: 戻り値: cos, v: ローカル座標系のベクトル
double cosTheta(const Vec3& v){
    return v.y;
};

class Mirror : public Material{
    public:
        Mirror() {};

        Vec3 sample(const Vec3& wo, Vec3& wi, double& pdf) const {
            wi = reflect(wo, Vec3(0, 1, 0));

            pdf = 1.0;
            return 1/cosTheta(wi) * Vec3(1);
        };
};

#endif
