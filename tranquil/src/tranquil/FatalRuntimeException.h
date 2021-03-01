#pragma once

#include <exception>

class FatalRuntimeException : public std::exception {
private:
    std::string errorMessage;
public:
    FatalRuntimeException() {
        this->errorMessage = "A fatal runtime error occured in the Javascript runtime";
    }
    FatalRuntimeException(const std::string& errorMessage) {
        this->errorMessage = errorMessage;
    }
    const char* what() const noexcept override {
        return this->errorMessage.c_str();
    }
};