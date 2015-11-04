#ifndef UTIL_LOGSTREAM_H
#define UTIL_LOGSTREAM_H

#include <util/log.h>
#include <ostream>
#include <string>
#include <sstream>

class _set_log_level_func;

class logstream {
    std::stringstream ss;
    std::string buf;
    std::ostream &out;
    unsigned int log_level = LOG_D;

    void flush_ss() {
        buf += ss.str();
        ss.str("");
        if (buf.rfind("\n") != std::string::npos) {
            time_t current_t;
            time(&current_t);
            int log_time = difftime(current_t, begin_t);
            char header[20];
            printl_header(header, log_time, log_level);
            out << header << buf.substr(0, buf.rfind("\n") + 1);
            buf = buf.substr(buf.rfind("\n") + 1);
        }
    }

public:
    logstream(std::ostream &_out) : ss(), out(_out) {}
    
    void set_log_level(unsigned int ll) {
        log_level = ll;
    }

    template <class T>
    logstream &operator<<(const T &t) {
        if (log_level < min_log_level) {
            return *this;
        }
        ss << t;
        flush_ss();
        return *this;
    }

    logstream &operator<<(std::ostream &(*f)(std::ostream &)) {
        f(ss);
        flush_ss();
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
