#pragma once

#include "CHEL/pch.h"
#include "CHEL/FatalRuntimeException.h"
#include "value.h"

namespace JS {
    class String : public Value {
    public:
        using Value::Value;
        using Value::operator JsValueRef;
        /**
         * Create a JavaScript string from a cstring
         *
         * @param string a cstring value, cannot be null
         */
        explicit String(const char* string);
        /**
         * Get a C++ string from a JavaScript value
         *
         * @return a string
         */
        std::string FromJS();
    };
}