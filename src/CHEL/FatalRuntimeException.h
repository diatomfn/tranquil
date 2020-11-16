#pragma once

#include <exception>

class FatalRuntimeException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Fatal javascript runtime error has occurred";
    }
};