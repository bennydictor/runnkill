#ifndef MATH_ORTOHEDRON_H
#define MATH_ORTOHEDRON_H

#include <math/vecmath.h>


#ifdef __cplusplus
#include <math/vec3.h>

struct ortohedron {
    vec3<float> p1, p2, p3, p4;
    vec3f vec_table[4];
    ortohedron(vec3<float> a, vec3<float> b, vec3<float> c, vec3<float> d);
    operator vec3f *();
};

#else

typedef vec3f *ortohedron;

#endif

#endif // MATH_ORTOHEDRON_H
