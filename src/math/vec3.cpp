#include <math/vec3.h>


template <>
vec3<float>::operator vec3f() {
    return reinterpret_cast<vec3f>(this);
}
