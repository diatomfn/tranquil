#pragma once

#include "CHEL/pch.h"
#include "CHEL/FatalRuntimeException.h"

#include "value.h"

namespace JS {
    class Boolean : public Value {
    public:
        using Value::Value;
        using Value::operator JsValueRef;
        /*
         * Create a JavaScript boolean from a bool
         *
         * @param string a cstring value, cannot be null
         */
        explicit Boolean(bool boolean);
        /**
         * Get a bool from a JavaScript value
         */
        operator bool () const;
    };
}