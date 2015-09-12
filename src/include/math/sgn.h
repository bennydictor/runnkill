#ifndef SGN_H
#define SGN_H


template <class T>
int sgn(const T &v) {
    return (0 < v ? 1 : (v < 0 ? -1 : 0));
}


#endif // SFN_H
