#ifndef COMMON_FORMAT_H
#define COMMON_FORMAT_H
#include <cstdio>
#include <memory>

template<typename ...Args>
std::string format(const std::string &fmt, Args ...args)
{
    unsigned long size = std::snprintf(nullptr, 0u, fmt.c_str(), args...) + 1;
    std::unique_ptr<char[]> ptr{new char[size]};
    std::snprintf(ptr.get(), size, fmt.c_str(), args...);
    return std::string(ptr.get());
}

#endif