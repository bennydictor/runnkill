#ifndef _UTIL_SMTH_H_
#define _UTIL_SMTH_H_

template <class T>

T get_mid(T low, T hig, float proportion) {
    return low + proportion * (hig - low);
}
    
#endif //_UTIL_SMTH_H_
