#ifndef VEC3_H
#define VEC3_H
#include <cmath>
#include <iostream>

class Vec3 {
   public:
    double x;
    double y;
    double z;

    Vec3() { x = y = z = 0; };
    Vec3(double _x) { x = y = z = _x; };
    Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}

    Vec3 operator-() const { return Vec3(-x, -y, -z); };

    Vec3& operator+=(const Vec3& v) {
        x += v.x;
        y += v.y;
        z += v.z;

        return *this;
    };

    Vec3& operator-=(const Vec3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;

        return *this;
    };

    Vec3& operator*=(const Vec3& v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;

        return *this;
    };

    Vec3& operator/=(const Vec3& v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;

        return *this;
    };

    double length() const { return std::sqrt(x * x + y * y + z * z); };

    double length2() const { return x * x + y * y + z * z; };
};

// ベクトルの四則演算
Vec3 operator+(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vec3 operator-(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vec3 operator*(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

Vec3 operator/(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

// ベクトルとスカラーの四則演算
Vec3 operator+(const Vec3& v, double k) {
    return Vec3(v.x + k, v.y + k, v.z + k);
}

Vec3 operator+(double k, const Vec3& v) { return v + k; }

Vec3 operator-(const Vec3& v, double k) {
    return Vec3(v.x + k, v.y + k, v.z + k);
}

Vec3 operator-(double k, const Vec3& v) {
    return Vec3(k - v.y, k - v.y, k - v.y);
}

Vec3 operator*(const Vec3& v, double k) {
    return Vec3(v.x * k, v.y * k, v.z * k);
}

Vec3 operator*(double k, const Vec3& v) { return v * k; }

Vec3 operator/(const Vec3& v, double k) {
    return Vec3(v.x / k, v.y / k, v.z / k);
}

Vec3 operator/(double k, const Vec3& v) {
    return Vec3(k / v.y, k / v.y, k / v.y);
}

// 比較演算子
bool operator==(const Vec3& a, const Vec3& b){
    if(a.x==b.x && a.y==b.y && a.z==b.z){
        return true;
    }
    else{
        return false;
    }
}

// 内積
double dot(const Vec3& v1, const Vec3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// 外積
Vec3 cross(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
                v1.x * v2.y - v1.y * v2.x);
}

// 正規化
Vec3 normalize(const Vec3& v) { return v / v.length(); }

// コンソール出力
std::ostream& operator<<(std::ostream& stream, const Vec3& v) {
    stream << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return stream;
}

// 正規直行基底を求める (v1はノーマライズしなくていいのか？？？？)
void orthonormalBasis(const Vec3& v1, Vec3& v2, Vec3& v3) {
    if (std::abs(v1.x) > 0.9)
        v2 = Vec3(0, 1, 0);
    else
        v2 = Vec3(1, 0, 0);

    v2 = normalize(v2 - dot(v1, v2) * v1);
    v3 = cross(v1, v2);
}

Vec3 worldToLocal(const Vec3& v, const Vec3& s, const Vec3& t, const Vec3& n) {
    return Vec3(dot(v, s), dot(v, t), dot(v, n));
}

Vec3 localToWorld(const Vec3& v, const Vec3& s, const Vec3& t, const Vec3& n) {
    Vec3 a = Vec3(s.x, n.x, t.x);
    Vec3 b = Vec3(s.y, n.y, t.y);
    Vec3 c = Vec3(s.z, n.z, t.z);

    return Vec3(dot(v, a), dot(v, b), dot(v, c));
}

#endif