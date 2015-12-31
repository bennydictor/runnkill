#include <util/logstream.h>


using namespace std;

void logstream::flush_ss() {
    buf += ss.str();
    ss.str("");
    if (buf.rfind("\n") != string::npos) {
        time_t current_t;
        time(&current_t);
        int log_time = difftime(current_t, begin_t);
        char header[20];
        printl_header(header, log_time, log_level);
        out << header << buf.substr(0, buf.rfind("\n") + 1);
        buf = buf.substr(buf.rfind("\n") + 1);
    }
}

void logstream::set_log_level(unsigned int ll) {
    log_level = ll;
}

logstream &logstream::operator<<(ostream &(*f)(ostream &)) {
    f(ss);
    flush_ss();
    return *this;
}

inline _set_log_level_func setloglevel(unsigned int ll) {
    return _set_log_level_func(ll);
}

logstream &logstream::operator<<(const _set_log_level_func &f) {
    set_log_level(f.ll);
    return *this;
}

#undef cerr
logstream lout(cerr);
