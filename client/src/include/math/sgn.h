#ifndef MATH_SGN_H
#define MATH_SGN_H


template <class T>
int sgn(const T &v) {
    return (0 < v ? 1 : (v < 0 ? -1 : 0));
}


#endif // MATH_SFN_H
