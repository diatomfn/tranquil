#pragma once

#include "CHEL/pch.h"
#include "CHEL/FatalRuntimeException.h"

#include "value.h"

namespace JS {
    class Number : public Value {
    public:
        using Value::Value;
        using Value::operator JsValueRef;
        /**
         * @brief Create a JavaScript number from a double
         *
         * @param double
         */
        explicit Number(double number);
        operator double () const;
    };
}