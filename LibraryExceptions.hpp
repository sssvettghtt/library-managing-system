#pragma once
#include <exception>
#include <string>

class LibraryExceptions : public std::exception
{
    std::string msg;

public:
    LibraryExceptions(const std::string &m) : msg(m) {}
    const char *what() const noexcept override
    {
        return msg.c_str();
    }
};