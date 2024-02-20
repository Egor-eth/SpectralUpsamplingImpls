#ifndef INCLUDE_SPECTRAL_INTERNAL_COMMON_UTIL_H
#define INCLUDE_SPECTRAL_INTERNAL_COMMON_UTIL_H

namespace spec {

    void init_progress_bar(unsigned maxcount, unsigned minupdate = 1);

    void finish_progress_bar();

    void print_progress(unsigned count, bool force = false);


    bool is_little_endian();

    void serial_copy(const char *src, char *dst, unsigned size);

}

#endif