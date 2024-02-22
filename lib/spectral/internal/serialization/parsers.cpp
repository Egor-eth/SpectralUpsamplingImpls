#include <internal/serialization/parsers.h>

namespace spec {

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
    unsigned parse(const std::string &str)
    {
        return stoul(str);
    }

    template<>
    unsigned long parse(const std::string &str)
    {
        return stoul(str);
    }

    template<>
    unsigned long long parse(const std::string &str)
    {
        return stoull(str);
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