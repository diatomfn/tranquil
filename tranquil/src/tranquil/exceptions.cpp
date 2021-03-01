#include "exceptions.h"

namespace tranquil::Exceptions {
    void InvalidArgument() {
        tranquil::Runtime::ThrowException("Invalid argument type.");
    }
    
    void ClassNewKeyword() {
        JsValueRef error;
        if (JsCreateTypeError(tranquil::Value("Class constructor cannot be called without the new keyword"), &error) != JsNoError)
            throw FatalRuntimeException();
        tranquil::Runtime::ThrowException(error);
    }
}