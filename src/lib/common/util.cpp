#include "util.h"
#include <iostream>


namespace spec {
    void init_progress_bar() {
        std::cout << std::endl;
    }

    void finish_progress_bar() {
        std::cout << "\033[A\033[2K\r";
    }

    void print_progress(float p) 
    {
        const int bar_length = 100;
        int progress_points = bar_length * p;
        std::cout << "\033[F[";
        std::cout << std::string(progress_points, '*') + std::string(bar_length - progress_points, ' ');
        std::cout << "]\n";
    }

    bool is_little_endian()
    {
        int i = 1;
        return *(reinterpret_cast<char *>(&i));
    }

    void serial_copy(const char *src, char *dst, unsigned size)
    {
        if(is_little_endian()) {
            for(unsigned i = 0; i < size; ++i) {
                dst[i] = src[size - i - 1];
            }
        }  
        else {
            std::copy(src, src + size, dst);
        }
    }

}