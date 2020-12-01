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
         * @brief Get the result of a promise not created with the wrapper
         */
        static JS::Value GetPromiseResult(JsValueRef value);

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

        /**
         * @brief Get the promise state
         * 
         * @return the current state of the promise
         */
        JsPromiseState GetState();

        /**
         * @brief Get the promise state
         * 
         * @param value the value to get the state from
         * 
         * @return the current state of the promise
         */
        static JsPromiseState GetState(JsValueRef value);
    private:
        JsValueRef resolveFunc;
        JsValueRef rejectFunc;
    };
}