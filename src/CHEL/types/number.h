#pragma once

#include "CHEL/pch.h"
#include "CHEL/FatalRuntimeException.h"

#include "value.h"

namespace JS {
    class Number : public Value {
    public:
        using Value::Value;
        using Value::operator JsValueRef;
        /*
         * Create a JavaScript number from a double
         *
         * @param string a cstring value, cannot be null
         */
        explicit Number(double number);
        /**
         * Get a double from a JavaScript number
         */
        operator double () const;
    };
}