#include "exceptions.h"
#include "common.h"

#include <fmt/format.h>

namespace JS::Exceptions {
    void InvalidArgument(JsValueType type) {
        JS::Runtime::ThrowException(fmt::format("Argument type is invalid, expected {}", JS::Value::GetTypeString(type)).c_str());
    }
    
    void ClassNewKeyword(const char* className) {
        JS::Runtime::ThrowException(fmt::format("Class constructor {} cannot be invoked without 'new'", className).c_str());
    }
}