#ifndef UTIL_H
#define UTIL_H
#include <list>
#include <vector>

template <typename T>
T clamp(T x, T xmin, T xmax) {
    if (x < xmin)
        return xmin;
    else if (x > xmax)
        return xmax;
    else
        return x;
}

// 行列式
double det(double A[3][3]) {
    double result = 0.0;
    result += (A[0][0] * A[1][1] * A[2][2] + A[0][1] * A[1][2] * A[2][0] +
               A[0][2] * A[1][0] * A[2][1]);
    result -= (A[0][2] * A[1][1] * A[2][0] + A[0][1] * A[1][0] * A[2][2] +
               A[0][0] * A[1][2] * A[2][1]);

    return result;
}

// クラメルの公式
std::vector<double> Cramer(const Vec3& origin, const Vec3& edge1,
                           const Vec3& edge2, const Ray& ray) {
    Vec3 r = ray.origin;
    Vec3 d = ray.direction;
    double M[3][3] = {{edge1.x, edge2.x, -d.x},
                      {edge1.y, edge2.y, -d.y},
                      {edge1.z, edge2.z, -d.z}};
    double C[3] = {r.x - origin.x, r.y - origin.y, r.z - origin.z};
    double det_origin = det(M);

    double alpha_M[3][3] = {{C[0], M[0][1], M[0][2]},
                            {C[1], M[1][1], M[1][2]},
                            {C[2], M[2][1], M[2][2]}};
    double alpha = det(alpha_M) / det_origin;

    double beta_M[3][3] = {{M[0][0], C[0], M[0][2]},
                           {M[1][0], C[1], M[1][2]},
                           {M[2][0], C[2], M[2][2]}};
    double beta = det(beta_M) / det_origin;

    double t_M[3][3] = {{M[0][0], M[0][1], C[0]},
                        {M[1][0], M[1][1], C[1]},
                        {M[2][0], M[2][1], C[2]}};
    double t = det(t_M) / det_origin;

    std::vector<double> answer = {alpha, beta, t};

    return answer;
}

#endif