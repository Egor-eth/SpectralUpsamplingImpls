#include <internal/common/util.h>
#include <iostream>


namespace spec {
    unsigned _progress_max_count = 1;
    unsigned _progress_min_update_step = 1;
    unsigned _prev_val = 0;

    void init_progress_bar(unsigned maxcount, unsigned minupdate) {
        _progress_max_count = maxcount;
        _progress_min_update_step = minupdate;
        _prev_val = 0;
        std::cout << std::endl;
    }

    void finish_progress_bar() {
        print_progress(_progress_max_count, true);
        //std::cout << "\033[A\033[2K\r";
    }

    void print_progress(unsigned count, bool force) 
    {
        static const unsigned bar_length = 100;
        if(force || count - _prev_val > _progress_min_update_step) {
            _prev_val = (count / _progress_min_update_step) * _progress_min_update_step;
            unsigned progress_points = bar_length * (count / static_cast<float>(_progress_max_count));
            std::cout << "\033[F[";
            std::cout << std::string(progress_points, '*') + std::string(bar_length - progress_points, ' ');
            std::cout << "] " << count << "/" << _progress_max_count << " - " << progress_points << "%\n";
        }
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

    void convert_to_native_order(const char *src, char *dst, unsigned size, bool from_big_endian)
    {
        if(is_little_endian() ^ !from_big_endian) {
            for(unsigned i = 0; i < size; ++i) {
                dst[i] = src[size - i - 1];
            }
        }  
        else {
            std::copy(src, src + size, dst);
        }
    }

    void convert_from_native_order(const char *src, char *dst, unsigned size, bool to_big_endian)
    {
        convert_to_native_order(src, dst, size, to_big_endian);
    }

}