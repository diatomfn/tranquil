#pragma once

#include "CHEL/pch.h"
#include "CHEL/FatalRuntimeException.h"

#include "value.h"

namespace JS {
    class Promise : public Value {
    public:
        using Value::Value;
        /**
         * @brief Create a new Javascript promise
         */
        Promise();

        /**
         * @brief Resolve a Javascript promise
         * 
         * @param value the value to resolve
         */
        void Resolve(JsValueRef value);

        /**
         * @brief Reject a Javascript promise
         * 
         * @param value the value to reject
         */
        void Reject(JsValueRef value);
    private:
        JsValueRef resolveFunc;
        JsValueRef rejectFunc;
    };
}