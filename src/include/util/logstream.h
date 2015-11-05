#ifndef UTIL_LOGSTREAM_H
#define UTIL_LOGSTREAM_H

#include <util/log.h>
#include <iostream>
#include <string>
#include <sstream>

class _set_log_level_func;

class logstream {
    std::stringstream ss;
    std::string buf;
    std::ostream &out;
    unsigned int log_level = LOG_D;

    void flush_ss();

public:
    logstream(std::ostream &_out) : ss(), out(_out) {}

    template <class T>
    logstream &operator<<(const T &t) {
        if (log_level < min_log_level) {
            return *this;
        }
        ss << t;
        flush_ss();
        return *this;
    }

    void set_log_level(unsigned int ll);
    logstream &operator<<(std::ostream &(*f)(std::ostream &));
    logstream &operator<<(const _set_log_level_func &f);
};

class _set_log_level_func {
    unsigned int ll;
public:
    _set_log_level_func(unsigned int _ll) : ll(_ll) {}
    friend logstream &logstream::operator<<(const _set_log_level_func &f);
};

inline _set_log_level_func setloglevel(unsigned int ll);

extern logstream lout;

#define cout lout
#define cerr lout

#endif // UTIL_LOGSTREAM_H
