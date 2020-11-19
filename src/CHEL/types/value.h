#pragma once

#include "CHEL/pch.h"
#include "CHEL/FatalRuntimeException.h"

namespace JS {
    class Value {
    public:
        /**
         * Create a JavaScript value from a Chakra value
         *
         * @param value JavaScript value
         */

        explicit Value(JsValueRef value);
        /**
         * Create an undefined JavaScript value
         */
        explicit Value();

        operator JsValueRef () const;
    protected:
        JsValueRef value = JS_INVALID_REFERENCE;
    };
}