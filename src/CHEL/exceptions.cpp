#include "exceptions.h"
#include "common.h"

#include <fmt/format.h>

namespace JS::Exceptions {
    void InvalidArgument() {
        JS::Runtime::ThrowException("Invalid argument type.");
    }
    
    void ClassNewKeyword() {
        JsValueRef error;
        if (JsCreateTypeError(JS::Value("Class constructor cannot be called without the new keyword"), &error) != JsNoError)
            throw FatalRuntimeException();
        JS::Runtime::ThrowException(error);
    }
}