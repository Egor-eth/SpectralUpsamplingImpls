#ifndef COMMON_CSV_H
#define COMMON_CSV_H
#include "csvutil.h"
#include <unordered_map>
#include <vector>
#include <istream>

namespace csv {
    template<typename Head, typename... Columns>
    std::vector<std::tuple<Head, Columns...>> load_as_vector(std::istream &stream, char delim = ',')
    {
        std::vector<std::tuple<Head, Columns...>> res;
        while(stream) {
            res.push_back(parse_line<Head, Columns...>(stream, delim));
        }
        return res;
    }
}

#endif