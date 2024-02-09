#ifndef SERIALIZATION_PARSERS_H
#define SERIALIZATION_PARSERS_H
#include <string>
#include <sstream>

namespace spec {

    template<typename T>
    T parse(const std::string &str)
    {
        std::stringstream ss(str);
        T val;
        ss >> val;
        return val;
    }

}

#endif