#ifndef COMMON_CSVUTIL_H
#define COMMON_CSVUTIL_H
#include <vector>
#include <tuple>
#include <string>
#include <istream>
#include <stdexcept>
#include "parsers.h"

namespace csv {

    template<typename Head>
    std::tuple<Head> parse_line(const std::string &str, char delim = ',', size_t pos = 0)
    {
        if(pos == std::string::npos) throw std::invalid_argument("Empty substring");
        size_t nextpos = str.find(delim, pos);
        std::string word = str.substr(pos, nextpos);
        return {parse<Head>(word)};
    }

    template<typename Head, typename... Columns>
    std::enable_if_t<(sizeof...(Columns) > 0), std::tuple<Head, Columns...>> 
    parse_line(const std::string &str, char delim = ',', size_t pos = 0)
    {
        if(pos == std::string::npos) throw std::invalid_argument("Empty substring");
        size_t nextpos = str.find(delim, pos);
        std::string word = str.substr(pos, nextpos);
        std::tuple<Head> t1{parse<Head>(word)};
        if(nextpos != std::string::npos) nextpos += 1;
        std::tuple<Columns...> t2 = parse_line<Columns...>(str, delim, nextpos);
        return std::tuple_cat<>(t1, t2);
    }

    template<typename Head, typename... Columns>
    std::tuple<Head, Columns...> parse_line(std::istream &stream, char delim = ',')
    {
        std::string str;
        std::getline(stream, str);
        return parse_line<Head, Columns...>(str, delim);
    }

}

#endif