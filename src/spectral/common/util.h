#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

namespace spec {

    void init_progress_bar(unsigned maxcount);

    void finish_progress_bar();

    void print_progress(unsigned count);


    bool is_little_endian();

    void serial_copy(const char *src, char *dst, unsigned size);

}

#endif