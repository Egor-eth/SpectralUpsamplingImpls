#ifndef INCLUDE_SPECTRAL_INTERNAL_SERIALIZATION_CSV_H
#define INCLUDE_SPECTRAL_INTERNAL_SERIALIZATION_CSV_H
#include <spectral/internal/serialization/parsers.h>
#include <unordered_map>
#include <vector>
#include <istream>
#include <tuple>
#include <string>
#include <optional>

namespace spec {
    namespace csv {
        constexpr char COMMENT_SYM = '#';

        /**
         *  Returns empty optional on empty input or on comments.
         * Otherwise throws exception
         */

        template<typename Head>
        std::optional<std::tuple<Head>> parse_line(const std::string &str, char delim = ',', size_t pos = 0, bool ignore_comments = true)
        {
            if(pos == std::string::npos || (ignore_comments && str[pos] == COMMENT_SYM)) return {};
            size_t nextpos = str.find(delim, pos);
            std::string word = str.substr(pos, nextpos);
            return {parse<Head>(word)};
        }

        template<typename Head, typename... Columns>
        std::enable_if_t<(sizeof...(Columns) > 0), std::optional<std::tuple<Head, Columns...>>> 
        parse_line(const std::string &str, char delim = ',', size_t pos = 0, bool ignore_comments = true)
        {
            if(pos == std::string::npos || (ignore_comments && str[pos] == COMMENT_SYM)) return {};
            size_t nextpos = str.find(delim, pos);
            std::string word = str.substr(pos, nextpos);
            std::tuple<Head> t1{parse<Head>(word)};
            if(nextpos != std::string::npos) nextpos += 1;
            std::optional<std::tuple<Columns...>> t2 = parse_line<Columns...>(str, delim, nextpos, ignore_comments);
            if(!t2) throw std::runtime_error("Unfinished csv line");
            return std::tuple_cat<>(t1, *t2);
        }

        template<typename Head, typename... Columns>
        std::optional<std::tuple<Head, Columns...>> parse_line(std::istream &stream, char delim = ',', bool ignore_comments = true)
        {
            std::string str;
            std::getline(stream, str);
            return parse_line<Head, Columns...>(str, delim, ignore_comments);
        }


        template<typename Head, typename... Columns>
        std::vector<std::tuple<Head, Columns...>> load_as_vector(std::istream &stream, char delim = ',', bool ignore_comments = true)
        {  
            std::vector<std::tuple<Head, Columns...>> res;
            while(stream.peek() != EOF) {
                auto data = parse_line<Head, Columns...>(stream, delim, ignore_comments);
                if(data) {
                    res.push_back(*data);
                }
            }
            return res;
        }
    }
}

#endif