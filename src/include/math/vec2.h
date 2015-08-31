#ifndef VEC2_H
#define VEC2_H


#include <vector>
#include <iostream>

#include <util/sgn.h>

template <typename T>
struct vec2 {
    T x, y;

    vec2() : x(0), y(0) {}
    vec2(T _x, T _y) : x(_x), y(_y) {}

    T dot(const vec2<T> &v) const;
    T crs(const vec2<T> &v) const;
    T sdot(const vec2<T> &v) const;
    T scrs(const vec2<T> &v) const;
    T sqlen() const;
};

typedef vec2<long long int> vec2ll;
typedef vec2<double> vec2d;
typedef std::vector<vec2<long long int>> polygonll;
typedef std::vector<vec2<double>> polygond;

template <class T>
vec2<T> operator+(const vec2<T> &a, const vec2<T> &b) {
    return vec2<T>(a.x + b.x, a.y - b.y);
}

template <class T>
vec2<T> operator-(const vec2<T> &a, const vec2<T> &b) {
    return vec2<T>(a.x - b.x, a.y - b.y);
}

template <class T>
vec2<T> operator*(T x, const vec2<T> &v) {
    return vec2<T>(x * v.x, x * v.y);
}

template <class T>
T vec2<T>::dot(const vec2<T> &v) const {
    return x * v.x + y * v.y;
}

template <class T>
T vec2<T>::crs(const vec2<T> &v) const {
    return x * v.y - y * v.x;
}

template <class T>
T vec2<T>::sdot(const vec2<T> &v) const {
    return sgn(dot(v));
}

template <class T>
T vec2<T>::scrs(const vec2<T> &v) const {
    return sgn(crs(v));
}

template <class T>
T vec2<T>::sqlen() const {
    return x * x + y * y;
}

template <class T>
std::istream &operator>>(std::istream &in, vec2<T> &v) {
    return in >> v.x >> v.y;
}

template <class T>
std::ostream &operator<<(std::ostream &out, const vec2<T> &v) {
    return out << "(" << v.x << ", " << v.y << ")";
}


#endif // VEC2_H
