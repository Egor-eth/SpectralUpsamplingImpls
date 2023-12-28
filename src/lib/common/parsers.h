#ifndef COMMON_PARSERS_H
#define COMMON_PARSERS_H

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

    template<>
    int parse(const std::string &str)
    {
        return stoi(str);
    }

    template<>
    long parse(const std::string &str)
    {
        return stol(str);
    }

    template<>
    long long parse(const std::string &str)
    {
        return stoll(str);
    }

    template<>
    float parse(const std::string &str)
    {
        return stof(str);
    }

    template<>
    double parse(const std::string &str)
    {
        return stod(str);
    }

    template<>
    long double parse(const std::string &str)
    {
        return stold(str);
    }

    template<>
    std::string parse(const std::string &str)
    {
        return str;
    }

}

#endif