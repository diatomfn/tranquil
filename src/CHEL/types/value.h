#pragma once

#include "CHEL/pch.h"
#include "CHEL/FatalRuntimeException.h"

namespace JS {
    class Value {
    public:
        /**
         * @brief Create a JavaScript value from a Chakra value
         *
         * @param value JavaScript value
         */
        explicit Value(JsValueRef value);
        /**
         * @brief Create an undefined JavaScript value
         */
        explicit Value();
        /**
         * @brief Get the type of a JS value as a string
         * 
         * @param value the javascript value
         * 
         * @return the type of the value as a string
         */
        const char* GetTypeString();
        /**
         * @brief Get the type of a Javascript value
         * 
         * @return JsValueType the type of the value
         */
        JsValueType GetType();

        static JsValueType GetType(JsValueRef value);

        static const char* GetTypeString(JsValueType type);
        static const char* GetTypeString(JsValueRef value);

        // Implicit conversion operators
        operator JsValueRef () const;
        operator std::string () const;
    protected:
        JsValueRef value = JS_INVALID_REFERENCE;
    };
}