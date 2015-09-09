#ifndef VEC3_H
#define VEC3_H


#include <iostream>
#include <cmath>
#include <common.h>
//#include <sgn.h>



template <typename T>
struct vec3 {
    T x, y, z;
    vec3() : x(0), y(0), z(0) {}
    vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
    vec3(vec3<T> b, vec3<T> e) : x(e.x - b.x), y(e.y - b.y), z(e.z - b.z) {};
    T dot(const vec3<T> &v) const;
    vec3<T> crs(const vec3<T> &v) const;
    T sdot(const vec3<T> &v) const;
    T sqlen() const;
    void resize(const float s);
};

struct ortohedron {
    vec3<float> p1, p2, p3, p4;
    ortohedron(vec3<float> a, vec3<float> b, vec3<float> c, vec3<float> d) {
        p1 = a;
        p2 = b;
        p3 = c;
        p4 = d;
    }
};

typedef vec3<long long int> vec3ll;
typedef vec3<double> vec3d;

template <class T>
vec3<T> operator+(const vec3<T> &a, const vec3<T> &b) {
    return vec3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template <class T>
vec3<T> operator-(const vec3<T> &a, const vec3<T> &b) {
    return vec3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template <class T>
vec3<T> operator*(T x, const vec3<T> &v) {
    return vec3<T>(x * v.x, x * v.y, x * v.z);
}

template <class T>
vec3<T> operator/(const vec3<T> &v, T x) {
    return vec3<T>(v.x / x, v.y / x, v.z / x);
}

template <class T>
T vec3<T>::dot(const vec3<T> &v) const {
    return x * v.x + y * v.y + z * v.z;
}

template <class T>
vec3<T> vec3<T>::crs(const vec3<T> &v) const {
    return vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}
/*
template <class T>
int vec3<T>::sdot(const vec3<T> &v) const {
    return sgn(dot(v));
}
*/
template <class T>
T vec3<T>::sqlen() const {
    return x * x + y * y + z * z;
}

template <class T>

void vec3<T>::resize(const float s) {
    float len = sqrt(this->sqlen());
    x /= len;
    y /= len;
    z /= len;
    (*this) = s * (*this);
}

template <class T>
std::ostream &operator<<(std::ostream &out, const vec3<T> &v) {
    return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

template <class T>
T dist(vec3<T> a, vec3<T> b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}

template <class T>
T dist(vec3<T> point, vec3<T> left, vec3<T> right) {
    for (int i = 0; i < 20; i++) {
        vec3<T> m1 = ((T)2 * left + right) / (T)3;
        vec3<T> m2 = (left + (T)2 * right) / (T)3;
        if (dist(m1, point) < dist(m2, point)) {
        
        }
    }
}

template <class T>
vec3<T> plain(vec3<T> p1, vec3<T> p2, vec3<T> p3) {
    return vec3<T>(p1, p2).crs(vec3<T>(p1, p3));
}

template <class T>

T _intersect_segment_plain(vec3<T> plain1, vec3<T>plain2, vec3<T> plain3, vec3<T> seg1, vec3<T> seg2) {
    vec3<T> coeffs = plain(plain1, plain2, plain3);
    T d = -plain1.dot(coeffs);
    vec3<T> seg_par = vec3<T>(seg1, seg2);
    if (coeffs.dot(seg_par) == 0) {
        if ((coeffs.dot(seg1) + d)  == 0) {
            return 0;
        }
        return -1;
    }
    return -((coeffs.dot(seg1) + d) / coeffs.dot(seg_par));
}

template <class T>

bool intersect_seg_plain(vec3<T> plain1, vec3<T>plain2, vec3<T> plain3, vec3<T> seg1, vec3<T> seg2, vec3<T> &res) {
    T res1 = _intersect_segment_plain(plain1, plain2, plain3, seg1, seg2);
    if (res1 >= 0 and res1 < 1) {
        res = seg1 + res1 * vec3<T>(seg1, seg2);    
    }
//    std::cerr << res1 << std::endl;
    return (!(res1 < 0 or res1 > 1 + EPS));
    
}

template <class T>

bool in_angle(vec3<T> plain1, vec3<T> plain2, vec3<T> plain3, vec3<T> point) {

    vec3<T> v1(plain1, plain2), v2(plain1, point), v3(plain1, plain3);
    vec3<T> crs1 = v1.crs(v2);
    vec3<T> crs2 = v1.crs(v3);
    vec3<T> crs3 = v2.crs(v3);
    return (crs1.dot(crs3) >= 0 and crs1.dot(crs2) >= 0);
}

template <class T>

bool intersect_seg_facet(vec3<T> plain1, vec3<T> plain2, vec3<T> plain3, vec3<T> seg1, vec3<T> seg2, vec3<T>& res) {
    vec3<T> plain4 = plain2 + vec3<T>(plain1, plain3);
//    std::cerr << ' ' << plain1 << ' ' << plain2 << ' ' << plain3 << ' ' << plain4 << '|' << seg1 << ' ' << seg2 << ';' << res << '\n';
    vec3<T> res1;
    if (!intersect_seg_plain(plain1, plain2, plain3, seg1, seg2, res1)) {
        return false;
    }
    bool ret = in_angle(plain1, plain2, plain3, res1) and in_angle(plain4, plain3, plain2, res1);
    if (ret) res = res1;
    return ret;
}


template <class T>

bool intersect_seg_ortohedron(ortohedron A, vec3<T> seg1, vec3<T> seg2, vec3<T>& res) {
    bool ret1 = intersect_seg_facet(A.p1, A.p2, A.p3, seg1, seg2, res) or
        intersect_seg_facet(A.p1, A.p2, A.p4, seg1, seg2, res) or
        intersect_seg_facet(A.p1, A.p3, A.p4, seg1, seg2, res);
    vec3<T> p5 = A.p2 + vec3<T>(A.p1, A.p3) + vec3<T>(A.p1, A.p4);
    bool ret2 = intersect_seg_facet(p5, A.p2 + vec3<T>(A.p1, A.p3), A.p3 + vec3<T>(A.p1, A.p4), seg1, seg2, res) or
        intersect_seg_facet(p5, A.p2 + vec3<T>(A.p1, A.p3) , A.p4 + vec3<T>(A.p1, A.p2), seg1, seg2, res) or
        intersect_seg_facet(p5, A.p3 + vec3<T>(A.p1, A.p4) , A.p4 + vec3<T>(A.p1, A.p2), seg1, seg2, res); 
    return (ret1 | ret2);

}

#endif // VEC3_H

