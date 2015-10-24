#ifndef UTIL_LOGSTREAM_H
#define UTIL_LOGSTREAM_H

#include <util/log.h>

#include <ostream>

class _set_log_level_func;

class logstream {
    std::ostream &out;
    unsigned int log_level = LOG_D;
public:
    logstream(std::ostream &_out) : out(_out) {}
    
    void set_log_level(unsigned int ll) {
        log_level = ll;
    }

    template <class T>
    logstream &operator<<(const T &t) {
        if (log_level < min_log_level) {
            return *this;
        }
        time_t current_t;
        time(&current_t);
        int log_time = difftime(current_t, begin_t);
        char header[20];
        printl_header(header, log_time, log_level);
        out << header << t;
        return *this;
    }

    logstream &operator<<(std::ostream &(*f)(std::ostream &)) {
        f(out);
        return *this;
    }

    logstream &operator<<(const _set_log_level_func &f);
};

class _set_log_level_func {
    unsigned int ll;
public:
    _set_log_level_func(unsigned int _ll) : ll(_ll) {}
    friend logstream &logstream::operator<<(const _set_log_level_func &f);
};

inline _set_log_level_func setloglevel(unsigned int ll) {
    return _set_log_level_func(ll);
}

logstream &logstream::operator<<(const _set_log_level_func &f) {
    set_log_level(f.ll);
    return *this;
}
logstream lout(std::cerr);

#define cout lout
#define cerr lout


#endif // UTIL_LOGSTREAM_H
